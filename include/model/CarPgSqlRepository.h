#pragma once

#include <vector>
#include "model/CarRepository.h"

class CarPgSqlRepository : public CarRepository
{
    int m_lastID;
    std::string m_host;
    std::string m_hostAddr;
    std::string m_port;
    std::string m_dbname;
    std::string m_user;
    std::string m_password;
    bool m_requireSsl;
    std::string m_connString;

    Car notValidCar() const;

public:
    std::vector<Car> readAll() override;
    Car create(const Car &entity) override;
    Car read(int id) override;
    Car update(const Car &entity) override;
    Car del(const Car &entity) override;

    std::vector<Car> read(uint start, uint limit) override;
    uint getCount() const override;
    int getNextID() override;

    CarPgSqlRepository() = default;
    bool configureConnection(std::string confFile = ".env");
    bool readConfigFile(std::string confFile);
};