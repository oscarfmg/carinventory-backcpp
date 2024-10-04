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

TEST(CarPgSqlRepositoryTest, CRUDAssertions) {
    CarPgSqlRepository repo;
    EXPECT_TRUE(repo.configureConnection());

    int originalCount = repo.getCount();

    /* CREATE */
    Car newCar(-1, "dos", "two", 2, "zwei", "deux", 2);
    Car pushedCar = repo.create(newCar);

    int newCount = repo.getCount();
    EXPECT_GT(newCount,originalCount) << "New Count should be greather than Original Count";
    EXPECT_GT(pushedCar.id,0) << "New ID should be > 0";

    auto printCar = [&](Car &car) {
        std::cerr << "\u001b[32m[          ] \u001b[33m" << car << "\u001b[0m" << std::endl;
    };

    /* READ */
    printCar(pushedCar);
    auto car = repo.read(pushedCar.id);
    printCar(pushedCar);
    EXPECT_EQ(car.id, pushedCar.id) << "Expected Read car be same than recently created";

    car = repo.read(1000);
    printCar(pushedCar);
    EXPECT_EQ(car.id, -1); // Id not found

    /* UPDATE */
    auto updateCar = Car(pushedCar.id,"tres","three",3,"drei","trois",3);
    car = repo.update(updateCar);
    printCar(car);
    EXPECT_EQ(car.id, pushedCar.id) << "Updated ID is not equal than expected";
    EXPECT_STREQ(car.model.c_str(),"tres") << "Model string is not equal than expected";

    /* DELETE */
    auto deletedCar = repo.del(car);
    printCar(deletedCar);
    EXPECT_EQ(deletedCar.id,car.id) << "Deleted ID doesn't matches expected ID";
    car = repo.read(deletedCar.id);
    EXPECT_EQ(car.id,-1) << "Deleted Car should not be found";
}