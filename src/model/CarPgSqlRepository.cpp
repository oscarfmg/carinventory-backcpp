#include <iostream>
#include <fstream>
#include <sstream>

#include "model/CarPgSqlRepository.h"
#include "fmt/core.h"
#include "fmt/os.h"
#include "pqxx/pqxx"

// TODO: Add worker thread

std::vector<Car> CarPgSqlRepository::readAll() {
    auto cars = std::vector<Car>();
    try {
        pqxx::connection c{m_connString};
        pqxx::work tx{c};
        for (auto [id, model, brand, kilometers, price, description, year] :
             tx.stream<int, std::string, std::string, int, std::string, std::optional<std::string>, std::optional<int>>(
                 "SELECT id,model,brand,kilometers,price,description,year FROM cars")) {
            cars.push_back(Car(id,model,brand,kilometers,price,description.value_or(""),year.value_or(-1)));
        }
    } catch (std::exception &ex) {
        std::cerr << ex.what() << "\n";
    }
    return cars;
}

Car CarPgSqlRepository::create(const Car &entity) {
    try {
        pqxx::connection c{m_connString};
        pqxx::work tx{c};
        std::string stmt = fmt::format(
            "do $$ "
            "begin "
            "call insert_car($1,$2,$3,$4,$5,$6); "
            "end $$"
        );
        c.prepare("create",stmt);
        pqxx::result res = tx.exec_prepared("create",
            entity.model, entity.brand, entity.kilometers, 
            entity.price,entity.description,entity.year);
    } catch (std::exception &ex) {

    }
    return Car();
}
Car CarPgSqlRepository::read(int id) {
    try
    {
        pqxx::connection c{m_connString};
        pqxx::work tx{c};
        auto res = tx.exec_params1(
            "SELECT id,model,brand,kilometers,price,description,year from cars WHERE cars.id = $1", id);
        return Car(res[0].as<int>(), res[1].as<std::string>(),
            res[2].as<std::string>(), res[3].as<int>(),
            res[4].as<std::string>(), res[5].as<std::string>(""),
            res[6].as<int>(-1));
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what() << "\n";
    }
    return Car(-1, "Not valid", "Not valid", -1, "Not valid");
}
Car CarPgSqlRepository::update(const Car &entity) {
    return Car();
}
Car CarPgSqlRepository::del(const Car &entity) {
    return Car();
}

std::vector<Car> CarPgSqlRepository::read(uint start, uint limit) {
    auto cars = std::vector<Car>();
    return cars;
}
uint CarPgSqlRepository::getCount() const {
    return 0;
}

int CarPgSqlRepository::getNextID() {
    return 1;
}

inline void ltrim(std::string &s) {
    s.erase(s.begin(),std::find_if(s.begin(),s.end(),[](unsigned char ch) {
        return not std::isspace(ch);
    }));
}

inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(),s.rend(),[](unsigned char ch) {
        return not std::isspace(ch);
    }).base(), s.end());
}

inline std::string trim(std::string s) {
    rtrim(s);
    ltrim(s);
    return s;
}

bool CarPgSqlRepository::readConfigFile(std::string confFile) {
    using namespace std;
    ifstream file(confFile);

    if (!file.is_open()) {
        cerr << fmt::format("Error opening file {}.\n", confFile);
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string var, val;
        getline(ss, var, '='); trim(var);
        getline(ss, val, '='); trim(val);
        std::transform(var.begin(), var.end(), var.begin(), ::toupper);

        if (var == "HOST")     m_host     = val;
        if (var == "HOSTADDR") m_hostAddr = val;
        if (var == "PORT")     m_port     = val;
        if (var == "DBNAME")   m_dbname   = val;
        if (var == "USER")     m_user     = val;
        if (var == "PASSWORD") m_password = val;
    }
    return true;
}

bool CarPgSqlRepository::configureConnection(std::string confFile) {
    if (not readConfigFile(confFile))
        return false;

    std::string connString = "postgresql://";
    if (not m_user.empty() or not m_password.empty()) {
        connString += m_user;
        if (not m_password.empty())
            connString += ":" + m_password;
        connString += "@";
    }
    if (not m_host.empty() or not m_hostAddr.empty()) {
        if (not m_host.empty())
            connString += m_host;
        else if (not m_hostAddr.empty())
            connString += m_hostAddr;
        if (not m_port.empty())
            connString += ":" + m_port;
    }
    if (not m_dbname.empty()) {
        connString += "/" + m_dbname;
    }

    try {
        pqxx::connection c{pqxx::zview(connString)};
        fmt::print("Connected to {}\n",c.dbname());
        m_connString = connString;
    } catch (std::exception &e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        std::cerr << connString << "\n";
        return false;
    }

    return true;
}