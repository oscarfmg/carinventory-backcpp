#include <iostream>
#include "model/Car.h"
#include "model/CarMemRepository.h"
#include "crow_all.h"

int main() {
    using namespace std;

    crow::SimpleApp app;
    CarMemRepository repo;
    repo.create({1,"uno","one",1,"eins"});
    repo.create({2,"dos","two",2,"zwei","deux",2});
    repo.create({3,"tres","three",3,"drei","trois",3});
    repo.create({4,"cuatro","four",4,"vier","quatre",4});

    CROW_ROUTE(app, "/car")([&](){
        auto all = repo.readAll();
        stringstream ss;
        ss << "[";
        for(auto it = all.begin(); it != all.end();) {
            ss << *it;
            ++it;
            if (it != all.end()) {
                ss << ",";
            }
        }
        ss << "]";
        return crow::response{"application/json",ss.str()};
    });

    CROW_ROUTE(app, "/car/<int>")([&](int id) {
        auto car = repo.read(id);
        return crow::response{"application/json",car.id == -1 ? "{}" : car.toString()};
    });

    auto json2Car = [](crow::json::rvalue &json) -> Car {
        try {
            Car car(json["id"].i(), 
                    json["model"].s(),
                    json["brand"].s(),
                    json["kilometers"].i(),
                    json["price"].s(),
                    (json.has("description") ? string(json["description"].s()) : ""),
                    json.has("year") ? json["year"].i() : -1);
            return car;
        } catch(const std::exception& e) {
            return Car {-2,"Not valid","Not valid",-2,"Not valid"};
        }
    };

    CROW_ROUTE(app, "/car")
        .methods("POST"_method)([&](const crow::request& req) {
            auto body = crow::json::load(req.body);
            if(!body) return crow::response(400);
            Car car = json2Car(body);
            if (car.id == -2) {
                return crow::response(400);
            }
            auto resp = repo.create(car);
            if (resp.id == -1) {
                return crow::response(400);
            }
            return crow::response{"application/json", resp.toString()};
    });

    CROW_ROUTE(app, "/car")
        .methods("PATCH"_method)([&](const crow::request& req){
            auto body = crow::json::load(req.body);
            if(!body) return crow::response(400);
            Car car = json2Car(body);
            if (car.id == -2) {
                return crow::response(400);
            }
            auto resp = repo.update(car);
            if (resp.id == -1) {
                return crow::response(400);
            }
            return crow::response{"application/json", resp.toString()};
        });

    CROW_ROUTE(app,"/car")
        .methods("DELETE"_method)([&](const crow::request& req){
            auto body = crow::json::load(req.body);
            if(!body) return crow::response(400);
            Car car = json2Car(body);
            if (car.id == -2) {
                return crow::response(400);
            }
            auto resp = repo.del(car);
            if (resp.id == -1) {
                return crow::response(400);
            }
            return crow::response{"application/json", resp.toString()};
        });

    app.port(18080).multithreaded().run();

    return 0;
}