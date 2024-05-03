#include "model/CarMemRepository.h"

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
