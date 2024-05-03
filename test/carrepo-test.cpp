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

}