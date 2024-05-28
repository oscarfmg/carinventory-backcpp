#include <gtest/gtest.h>
#include "model/Car.h"
#include "model/CarMemRepository.h"
#include <iostream>

TEST(CarMemRepositoryTest, CRUDAssertions) {
    Car car1;
    Car car2(2,"dos","two",2,"zwei","deux",2);
    Car car3(3,"tres","three",3,"drei","trois",3);
    Car car4(4,"cuatro","four",4,"vier","quatre",4);

    CarMemRepository repo;
    repo.create(car1);
    repo.create(car2);
    repo.create(car3);
    repo.create(car4);

    /* Read Car3 */
    auto repoRead = repo.read(3);
    EXPECT_EQ(repoRead.id,3);
    EXPECT_STREQ(repoRead.description.c_str(),"trois");

    /* Delete Car3 */
    auto repoDelete = repo.del(car3);
    EXPECT_EQ(repoDelete.id,3);
    EXPECT_STREQ(repoDelete.description.c_str(),"trois");
    repoDelete = repo.read(3);
    EXPECT_EQ(repoDelete.id,-1);
    EXPECT_STREQ(repoDelete.description.c_str(),"");

    /* Update Car4 */
    /** Read Car ID=4*/
    auto repoUpdate = repo.read(4);
    EXPECT_EQ(repoUpdate.id,4);
    EXPECT_STREQ(repoUpdate.description.c_str(),"quatre");
    /** Set Car3 => Car4 (Using car3.id=4) */
    car3.id = 4;
    repoUpdate = repo.update(car3);
    EXPECT_EQ(repoUpdate.id,4);
    EXPECT_STREQ(repoUpdate.description.c_str(),"trois");
    /** Read Car ID=4, values <=> Car3 */
    repoUpdate = repo.read(4);
    EXPECT_EQ(repoUpdate.id,4);
    EXPECT_STREQ(repoUpdate.description.c_str(),"trois");
    /** Read Car ID=4, Not found */
    repoUpdate = repo.read(3);
    EXPECT_EQ(repoUpdate.id,-1);
    EXPECT_STREQ(repoUpdate.description.c_str(),"");

    auto repoAll = repo.readAll();
    EXPECT_EQ(repoAll.size(), 3);
    for(const auto& car : repoAll) {
        std::cerr << "\u001b[32m[          ] \u001b[33m" << car << "\u001b[0m" << std::endl;
    }
}

TEST(CarMemRepositoryTest, DataPersistence) {
    Car car1(1,"uno","one",1,"eins");
    Car car2(2,"dos","two",2,"zwei","deux",2);
    Car car3(3,"tres","three",3,"drei","trois",3);
    Car car4(4,"cuatro","four",4,"vier","quatre",4);
    
    CarMemRepository repoOut("carrepo.json");
    repoOut.create(car1);
    repoOut.create(car2);
    repoOut.create(car3);
    repoOut.create(car4);

    EXPECT_EQ(repoOut.saveToDisk(),true);

    CarMemRepository repoIn("carrepo.json");
    EXPECT_EQ(repoIn.readFromDisk(),true);
    auto repoAll = repoIn.readAll();
    EXPECT_EQ(repoAll.size(), 4);
    for(const auto& car : repoAll) {
        std::cerr << "\u001b[32m[          ] \u001b[33m" << car << "\u001b[0m" << std::endl;
    }
}

TEST(CarMemRepositoryTest, SubRange)
{
    Car car1(1,"uno","one",1,"eins");
    Car car2(2,"dos","two",2,"zwei","deux",2);
    Car car3(3,"tres","three",3,"drei","trois",3);
    Car car4(4,"cuatro","four",4,"vier","quatre",4);

    CarMemRepository repo;
    repo.create(car1);
    repo.create(car2);
    repo.create(car3);
    repo.create(car4);

    auto cars = repo.read(1,2); // Read 2 cars, starting from 1
    EXPECT_EQ(cars.size(), 2) << "Expected 2 cars";
    EXPECT_EQ(cars[0].id, 2) << "Expected car id=2";
    EXPECT_EQ(cars[1].id, 3) << "Expected car id=3";

    cars = repo.read(2,0);
    EXPECT_EQ(cars.size(), 2) << "Expected 2 cars";
    EXPECT_EQ(cars[0].id, 3) << "Expected car id=3";
    EXPECT_EQ(cars[1].id, 4) << "Expected car id=4";
}