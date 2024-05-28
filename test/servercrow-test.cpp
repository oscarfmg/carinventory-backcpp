// #define CROW_ENABLE_DEBUG
// #define CROW_LOG_LEVEL 0

#include <gtest/gtest.h>
#include "server_crow/ServerCrow.h"
#include <fmt/core.h>

constexpr int HTTP_OK = 200;
constexpr int HTTP_BAD_REQ = 400;
constexpr int HTTP_NOT_FOUND = 404;

class ServerCrowTest : public testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    void json2Car (const crow::json::rvalue& json, Car& car) {
        for (const auto& key : json.keys()) {
                 if (key == "id") car.id = json["id"].i();
            else if (key == "model") car.model = json["model"].s();
            else if (key == "brand") car.brand = json["brand"].s();
            else if (key == "kilometers") car.kilometers = json["kilometers"].i();
            else if (key == "price") car.price = json["price"].s();
            else if (key == "description") car.description = json["description"].s();
            else if (key == "year") car.year = json["year"].i();
            else {
                car.id = car.kilometers = car.year = -2;
                car.model = car.brand = car.price = car.description = "Not valid";
                return;
            }
        }
    }

    CarMemRepository repo;
    ServerCrow app;
};

void ServerCrowTest::SetUp() {
    repo.create({1,"uno","one",1,"eins"});
    repo.create({2,"dos","two",2,"zwei","deux",2});
    repo.create({3,"tres","three",3,"drei","trois",3});
    repo.create({4,"cuatro","four",4,"vier","quatre",4});

    app.setRepo(&repo);
    app.init();
    app.loglevel(crow::LogLevel::ERROR);
    app.validate();
}

void ServerCrowTest::TearDown() {
    app.stop();
}

TEST_F(ServerCrowTest, REST_GetAll) {
    crow::request req;
    crow::response res;

    req.url = "/car";
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_OK);

    CarMemRepository rep2;
    rep2.readFromString(res.body);

    auto all1 = repo.readAll();
    auto all2 = rep2.readAll();
    EXPECT_EQ(all1.size(), all2.size());
    EXPECT_EQ(repo.read(1).brand,repo.read(1).brand);
}

TEST_F(ServerCrowTest, REST_GetParams) {
    crow::request req;
    crow::response res;

    req.url = "/car";
    req.url_params = crow::query_string("start=1&limit=2",false);
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_OK);

    CarMemRepository repo2;
    repo2.readFromString(res.body);

    auto cars = repo2.readAll();
    EXPECT_EQ(cars.size(), 2) << "Expected 2 cars";
    EXPECT_EQ(cars[0].id, 2) << "Expected car id=2";
    EXPECT_EQ(cars[1].id, 3) << "Expected car id=3";
}

TEST_F(ServerCrowTest, REST_Get) {
    crow::request req;
    crow::response res;

    req.url = "/car/3";
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_OK);

    Car car;
    auto json = crow::json::load(res.body);
    json2Car(json,car);

    EXPECT_EQ(car.id, 3);
    EXPECT_EQ(car.model, "tres");
    EXPECT_EQ(car.brand, "three");
}

TEST_F(ServerCrowTest, REST_GetCount) {
    crow::request req;
    crow::response res;

    req.url = "/car/getCount";
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_OK);

    auto json = crow::json::load(res.body);
    auto count = json["count"].i();
    EXPECT_EQ(count, 4);
}

TEST_F(ServerCrowTest, REST_Post) {
    crow::request req;
    crow::response res;

    Car carReq(-1,"cinco","five",5,"fünf");
    req.url = "/car/5";
    req.body = carReq.toString();
    req.method = crow::HTTPMethod::POST;
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_OK);

    auto json = crow::json::load(res.body);
    Car carRes;
    json2Car(json,carRes);

    EXPECT_EQ(carRes.id, 5);
    EXPECT_EQ(carRes.model, "cinco");
    EXPECT_EQ(carRes.price, "fünf");

    req.body = "";
    req.method = crow::HTTPMethod::GET;
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_OK);

    json = crow::json::load(res.body);
    Car carGet;
    json2Car(json, carGet);

    EXPECT_EQ(carRes.id, carGet.id);
    EXPECT_EQ(carRes.model, carGet.model);
    EXPECT_EQ(carRes.kilometers, carGet.kilometers);
}

TEST_F(ServerCrowTest, REST_Patch) {
    crow::request req;
    crow::response res;

    req.url = "/car/2";
    req.body = fmt::format("{{\"model\":\"two\",\"price\":\"two\",\"description\":\"two\"}}");
    req.method = crow::HTTPMethod::PATCH;
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_OK);

    auto json = crow::json::load(res.body);
    Car carRes;
    json2Car(json,carRes);

    EXPECT_EQ(carRes.id, 2);
    EXPECT_EQ(carRes.model, "two");
    EXPECT_EQ(carRes.price, "two");
    EXPECT_EQ(carRes.description, "two");

    req.body = "";
    req.method = crow::HTTPMethod::GET;
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_OK);

    json = crow::json::load(res.body);
    Car carGet;
    json2Car(json, carGet);

    EXPECT_EQ(carRes.id, carGet.id);
    EXPECT_EQ(carRes.model, carGet.model);
    EXPECT_EQ(carRes.price, carGet.price);
    EXPECT_EQ(carRes.description, carGet.description);
    EXPECT_EQ(carRes.kilometers, carGet.kilometers);
}

TEST_F(ServerCrowTest, REST_Delete) {
    crow::request req;
    crow::response res;

    req.url = "/car/4";
    req.method = crow::HTTPMethod::DELETE;
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_OK);

    auto json = crow::json::load(res.body);
    Car carRes;
    json2Car(json,carRes);

    EXPECT_EQ(carRes.id, 4);
    EXPECT_EQ(carRes.model, "cuatro");
    EXPECT_EQ(carRes.price, "vier");
    EXPECT_EQ(carRes.description, "quatre");

    req.body = "";
    req.method = crow::HTTPMethod::GET;
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_NOT_FOUND);
}

TEST_F(ServerCrowTest, REST_NegativeTest) {
    crow::request req;
    crow::response res;

    req.url = "/";
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_NOT_FOUND);

    req.url = "/car/3";
    req.method = crow::HTTPMethod::POST;
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_BAD_REQ) << "Empty Body Test Failed!";

    req.body = fmt::format("{{\"id\":5}}");
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_BAD_REQ) << "Incomplete Body Test Failed!";

    Car car5{5,"cinco","five",5,"fünf"};
    req.body = car5.toString();
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_BAD_REQ) << "POST Existing Product Test Failed!";

    req.method = crow::HTTPMethod::PATCH;
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_BAD_REQ) << "PATCH Existing Product Test Failed!";

    req.method = crow::HTTPMethod::DELETE;
    req.url = "/car/5";
    app.handle_full(req,res);
    EXPECT_EQ(res.code, HTTP_NOT_FOUND) << "DELETE Non Existing Product Test Failed!";
}