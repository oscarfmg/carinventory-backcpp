#pragma once

#include <vector>
#include <unordered_map>
#include "model/CarRepository.h"

class CarMemRepository : CarRepository {
    std::unordered_map<int, Car> m_cars;
public:
    std::vector<Car> readAll() override;
    Car create(const Car& entity) override;
    Car read(int id) override;
    Car update(const Car& entity) override;
    Car del(const Car& entity) override;
};