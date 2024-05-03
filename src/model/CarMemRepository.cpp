#include "model/CarMemRepository.h"
#include <iostream>
#include "fmt/core.h"
#include "fmt/os.h"

CarMemRepository::CarMemRepository(std::string path) 
    : m_path(path) {}

std::vector<Car> CarMemRepository::readAll(){
    std::vector<Car> all;
    all.reserve(m_cars.size());
    for(const auto&kv : m_cars) {
        all.push_back(kv.second);
    }
    return all;
}

Car CarMemRepository::create(const Car& entity) {
    m_cars[entity.id] = entity;
    return entity;
}

Car CarMemRepository::read(int id) {
    if (m_cars.contains(id)) {
        return m_cars[id];
    }
    return Car(-1,"Not valid","Not valid",-1,"Not valid");
}

Car CarMemRepository::update(const Car& entity) {
    if (m_cars.contains(entity.id)) {
        m_cars[entity.id] = entity;
        return entity;
    }
    return Car(-1,"Not valid","Not valid",-1,"Not valid");
}

Car CarMemRepository::del(const Car& entity){
    if (m_cars.contains(entity.id)) {
        m_cars.erase(entity.id);
        return entity;
    }
    return Car(-1,"Not valid","Not valid",-1,"Not valid");
}

bool CarMemRepository::readFromDisk() {
    return true;
}

bool CarMemRepository::saveToDisk() {
    auto out = fmt::output_file(m_path.c_str(), fmt::file::WRONLY | fmt::file::CREATE);
    out.print("[");
    for (auto it = m_cars.begin(); it != m_cars.end();) {
        out.print("{}",it->second.toString());
        ++it;
        if (it != m_cars.end())
            out.print(",");
    }
    out.print("]");
    return true;
}