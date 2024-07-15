#pragma once

#include <vector>
#include <map>
#include "model/CarRepository.h"

class CarMemRepository : public CarRepository {
    std::map<int, Car> m_cars;
    std::string m_path;
    int m_lastID;
public:
    std::vector<Car> readAll() override;
    Car create(const Car& entity) override;
    Car read(int id) override;
    Car update(const Car& entity) override;
    Car del(const Car& entity) override;

    std::vector<Car> read(uint start, uint limit);
    uint getCount() const;
    int  getNextID();

    CarMemRepository() = default;
    CarMemRepository(std::string path);
    bool readFromDisk();
    bool saveToDisk();
    bool readFromString(const std::string_view json);
};