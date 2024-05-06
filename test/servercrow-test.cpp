#define CROW_ENABLE_DEBUG
#define CROW_LOG_LEVEL 0

#include <gtest/gtest.h>
// #include "model/Car.h"
// #include "model/CarMemRepository.h"
// #include <iostream>
#include "server_crow/ServerCrow.h"
#include <fmt/core.h>

TEST(ServerCrowTest, REST_API_tests) {
    using namespace std;

    CarMemRepository repo;
    ServerCrow app;

    repo.create({1,"uno","one",1,"eins"});
    repo.create({2,"dos","two",2,"zwei","deux",2});
    repo.create({3,"tres","three",3,"drei","trois",3});
    repo.create({4,"cuatro","four",4,"vier","quatre",4});

    app.setRepo(&repo);
    app.init();
    app.validate();
    // app.debug_print();
    {
        crow::request req;
        crow::response res;

        req.url = "/car";
        app.handle_full(req,res);

        fmt::print("{}\n",res.body);

        CarMemRepository rep2;
        rep2.readFromString(res.body);

        auto all1 = repo.readAll();
        auto all2 = rep2.readAll();
        EXPECT_EQ(all1.size(), all2.size());
        EXPECT_EQ(repo.read(1).brand,repo.read(1).brand);
    }
    app.stop();
}