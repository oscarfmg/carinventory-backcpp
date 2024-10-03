#include <gtest/gtest.h>
#include "model/Car.h"
#include "model/CarPgSqlRepository.h"
#include <iostream>

TEST(CarPgSqlRepositoryTest, ConnectionTest) {
    CarPgSqlRepository repo;

    EXPECT_TRUE(repo.configureConnection());
}

TEST(CarPgSqlRepositoryTest, ReadAllCars) {
    CarPgSqlRepository repo;
    EXPECT_TRUE(repo.configureConnection());

    auto cars = repo.readAll();
    EXPECT_GT(cars.size(),0);
    for (const auto &car : cars) {
        std::cerr << "\u001b[32m[          ] \u001b[33m" << car << "\u001b[0m" << std::endl;
    }
}

TEST(CarPgSqlRepositoryTest, ReadCar) {
    CarPgSqlRepository repo;
    EXPECT_TRUE(repo.configureConnection());

    auto car = repo.read(2);
    std::cerr << "\u001b[32m[          ] \u001b[33m" << car << "\u001b[0m" << std::endl;
    EXPECT_GT(car.id,0);

    car = repo.read(1000);
    std::cerr << "\u001b[32m[          ] \u001b[33m" << car << "\u001b[0m" << std::endl;
    EXPECT_EQ(car.id, -1); // Id not found
}