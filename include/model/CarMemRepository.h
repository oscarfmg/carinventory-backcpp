#pragma once

#include <vector>
#include <unordered_map>
#include "model/CarRepository.h"

class CarMemRepository : public CarRepository {
    std::unordered_map<int, Car> m_cars;
    std::string m_path;
public:
    std::vector<Car> readAll() override;
    Car create(const Car& entity) override;
    Car read(int id) override;
    Car update(const Car& entity) override;
    Car del(const Car& entity) override;

    CarMemRepository() = default;
    CarMemRepository(std::string path);
    bool readFromDisk();
    bool saveToDisk();
    bool readFromString(const std::string_view json);
};