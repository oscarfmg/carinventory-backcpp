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
                 "SELECT id,model,brand,kilometers,price,description,year FROM cars ORDER BY id")) {
            cars.emplace_back(id,model,brand,kilometers,price,description.value_or(""),year.value_or(-1));
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
        auto res = tx.exec_params1(
            "SELECT * FROM public.insert_car($1::text, $2, $3, $4, $5, $6); ",
            pqxx::params{
            entity.model, entity.brand, entity.kilometers,
            entity.price,
            std::optional<std::string>(entity.description.empty() ? std::nullopt : std::optional<std::string>(entity.description)),
            std::optional<int>(entity.year == -1 ? std::nullopt : std::optional<int>(entity.year))});
        tx.commit();
        return Car(res["id"].as<int>(), res["model"].as<std::string>(),
                   res["brand"].as<std::string>(), res["kilometers"].as<int>(),
                   res["price"].as<std::string>(), res["description"].as<std::string>(""),
                   res["year"].as<int>(-1));
    } catch (std::exception &ex) {
        std::cerr << "Error creating Car: " << ex.what() << "\n";
    }

    return notValidCar();
}

Car CarPgSqlRepository::read(int id) {
    try
    {
        pqxx::connection c{m_connString};
        pqxx::work tx{c};
        auto res = tx.exec_params1(
            "SELECT id,model,brand,kilometers,price,description,year FROM cars WHERE cars.id = $1", 
            pqxx::params{id});
        return Car(res["id"].as<int>(), res["model"].as<std::string>(),
            res["brand"].as<std::string>(), res["kilometers"].as<int>(),
            res["price"].as<std::string>(), res["description"].as<std::string>(""),
            res["year"].as<int>(-1));
    }
    catch (std::exception &ex)
    {
        std::cerr << "Error reading Car: " << ex.what() << "\n";
    }

    return notValidCar();
}

Car CarPgSqlRepository::update(const Car &entity) {
    try {
        pqxx::connection c{m_connString};
        pqxx::work tx{c};
        auto res = tx.exec_params1(
            "SELECT * FROM public.update_car($1,$2,$3,$4,$5,$6,$7);",
            pqxx::params{
            entity.id, entity.model,entity.brand, entity.kilometers,
            entity.price, entity.description, entity.year});
        tx.commit();
        return Car(res["id"].as<int>(), res["model"].as<std::string>(),
                   res["brand"].as<std::string>(), res["kilometers"].as<int>(),
                   res["price"].as<std::string>(), res["description"].as<std::string>(""),
                   res["year"].as<int>(-1));
    } catch (std::exception &ex) {
        std::cerr << ex.what() << "\n";
    }

    return notValidCar();
}

Car CarPgSqlRepository::del(const Car &entity) {
    try {
        pqxx::connection c{m_connString};
        pqxx::work tx{c};
        auto res = tx.exec_params1(
            "SELECT * FROM public.delete_car($1);",
            pqxx::params{entity.id});
        tx.commit();
        return Car(res["id"].as<int>(), res["model"].as<std::string>(),
                   res["brand"].as<std::string>(), res["kilometers"].as<int>(),
                   res["price"].as<std::string>(), res["description"].as<std::string>(""),
                   res["year"].as<int>(-1));
    } catch (std::exception &ex) {
        std::cerr << ex.what() << "\n";
    }
    return notValidCar();
}

std::vector<Car> CarPgSqlRepository::read(uint start, uint limit) {
    std::vector<Car> cars;
    try
    {
        pqxx::connection c{m_connString};
        pqxx::work tx{c};
        std::string stmt = fmt::format("SELECT id,model,brand,kilometers,price,description,year "
                                            " FROM cars ORDER BY id OFFSET {} LIMIT {}",start,limit);
        for (auto [id, model, brand, kilometers, price, description, year] :
             tx.stream<int, std::string, std::string, int, std::string, std::optional<std::string>, std::optional<int>>(stmt)) {
            cars.emplace_back(id, model, brand, kilometers, price, description.value_or(""), year.value_or(-1));
        }
    }
    catch (std::exception &ex)
    {
        std::cerr << "Error reading Car: " << ex.what() << "\n";
    }
    return cars;
}

uint CarPgSqlRepository::getCount() const {
    try {
        pqxx::connection c{m_connString};
        pqxx::work tx{c};
        return tx.query_value<int>("SELECT public.get_cars_count()");
    } catch (std::exception &ex) {
        std::cerr << ex.what() << "\n";
    }
    return 0;
}

int CarPgSqlRepository::getNextID() {
    try {
        pqxx::connection c{m_connString};
        pqxx::work tx{c};
        return tx.query_value<int>("SELECT public.get_next_carID()");
    } catch (std::exception &ex) {
        std::cerr << ex.what() << "\n";
    }
    return 0;
}

Car CarPgSqlRepository::notValidCar() const {
    return Car(-1, "Not valid", "Not valid", -1, "Not valid");
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
        m_connString = connString;
    } catch (std::exception &e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        std::cerr << connString << "\n";
        return false;
    }

    return true;
}