#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "model/CarMemRepository.h"
#include "fmt/core.h"
#include "fmt/os.h"

CarMemRepository::CarMemRepository(std::string path) 
    : m_path(path)
    , m_lastID(0) {}

std::vector<Car> CarMemRepository::readAll(){
    std::vector<Car> all;
    all.reserve(m_cars.size());
    for(const auto&kv : m_cars) {
        all.push_back(kv.second);
    }
    return all;
}

std::vector<Car> CarMemRepository::read(uint start, uint limit) {
    std::vector<Car> cars;
    auto it = m_cars.begin();
    std::advance(it, start);
    for (int cnt = 0; it != m_cars.end() and (0 == limit or cnt < limit); ++it, ++cnt) {
        cars.push_back(it->second);
    }
    return cars;
}

uint CarMemRepository::getCount() const {
    return m_cars.size();
}

int CarMemRepository::getNextID() {
    return ++m_lastID;
}

Car CarMemRepository::create(const Car& entity) {
    if (auto search = m_cars.find(entity.id); search == m_cars.end()) {
        m_cars[entity.id] = entity;
        return entity;
    }
    return Car(-1,"Not valid","Not valid",-1,"Not valid");
}

Car CarMemRepository::read(int id) {
    if (auto search = m_cars.find(id); search != m_cars.end()) {
        return m_cars[id];
    }
    return Car(-1,"Not valid","Not valid",-1,"Not valid");
}

Car CarMemRepository::update(const Car& entity) {
    if (auto search = m_cars.find(entity.id); search != m_cars.end()) {
        m_cars[entity.id] = entity;
        return entity;
    }
    return Car(-1,"Not valid","Not valid",-1,"Not valid");
}

Car CarMemRepository::del(const Car& entity){
    if (auto search = m_cars.find(entity.id); search != m_cars.end()) {
        auto car = m_cars[entity.id];
        m_cars.erase(entity.id);
        return car;
    }
    return Car(-1,"Not valid","Not valid",-1,"Not valid");
}

bool CarMemRepository::readFromDisk() {
    using namespace std;
    ifstream file(m_path);
    if(!file.is_open()) {
        cerr << fmt::format("Error opening file {}.\n",m_path);
        return false;
    }
    file.seekg(0, std::ios::end);
    std::streampos len = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(len);
    file.read(&buffer[0], len);

    return readFromString(&buffer[0]);
}
bool CarMemRepository::readFromString(const std::string_view json) {
    using namespace std;

#ifndef __APPLE__
    stringstream ss;
    ss.rdbuf()->pubsetbuf(const_cast<char*>(json.data()), json.length());
#else
    stringstream ss;
    ss << json.data();
#endif

    string str;
    getline(ss, str, '[');
    while(!ss.eof()) {
        string obj;
        getline(ss,str,'{');
        if (str == "]") continue;
        
        getline(ss,obj,'}');
        auto removeQuotes = [](string str) -> string {
            str.erase(remove(str.begin(), str.end(), '\"'),str.end());
            return str;
        };
        auto tokenizeKV = [&] (string kv) -> pair<string,string> {
            stringstream sskv(kv);
            string k, v;
            getline(sskv,k,':');
            sskv >> v;
            return {removeQuotes(k),removeQuotes(v)};
        };
        auto tokenizeObj = [&] (string obj) -> map<string,string> {
            map<string,string> m;
            stringstream ssobj(obj);
            while(!ssobj.eof()) {
                string kv;
                getline(ssobj,kv,',');
                auto pkv = tokenizeKV(kv);
                m[pkv.first]=pkv.second;
            }
            return m;
        };
        auto mobj = tokenizeObj(obj);
        Car car(stoi(mobj["id"]),
                mobj["model"],
                mobj["brand"],
                stoi(mobj["kilometers"]),
                mobj["price"],
                mobj["description"],
                mobj["year"].empty()?-1:stoi(mobj["year"]));
        m_cars[car.id] = car;
    }

    m_lastID = m_cars.rbegin()->first;

    return true;
}

bool CarMemRepository::saveToDisk() {
    auto out = fmt::output_file(m_path.c_str(), fmt::file::WRONLY | fmt::file::CREATE | fmt::file::TRUNC);
    out.print("[");
    for (auto it = m_cars.begin(); it != m_cars.end();) {
        out.print("{}",it->second.toString());
        ++it;
        if (it != m_cars.end())
            out.print(",");
    }
    out.print("]");
    out.close();
    return true;
}
