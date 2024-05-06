#include <iostream>
#include <sstream>
#include "server_crow/ServerCrow.h"

void ServerCrow::init() {
    using namespace std;
    auto& app = *this;

    /* GET */
    CROW_ROUTE(app, "/car")([&](){
        auto all = m_repo->readAll();
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

    /* GET */
    CROW_ROUTE(app, "/car/<int>")([&](int id) {
        auto car = m_repo->read(id);
        return crow::response{"application/json",car.id == -1 ? "{}" : car.toString()};
    });

    auto json2Car = [](crow::json::rvalue &json, int id = -1) -> Car {
        try {
            Car car(id == -1 ? json["id"].i() : id, 
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

    /* POST */
    CROW_ROUTE(app, "/car/<int>")
        .methods("POST"_method)([&](const crow::request& req, int id) {
            auto body = crow::json::load(req.body);
            if(!body) return crow::response(400);
            Car car = json2Car(body, id);
            if (car.id == -2) {
                return crow::response(400);
            }
            auto resp = m_repo->create(car);
            if (resp.id == -1) {
                return crow::response(400);
            }
            return crow::response{"application/json", resp.toString()};
    });

    auto patchCar = [](const crow::json::rvalue& json, Car& car) {
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
    }};

    /* PATCH */
    CROW_ROUTE(app, "/car/<int>")
        .methods("PATCH"_method)([&](const crow::request& req, int id){
            auto body = crow::json::load(req.body);
            if(!body) return crow::response(400);
            Car car = m_repo->read(id);
            if (car.id == -1) {
                return crow::response(400);
            }
            patchCar(body, car);
            if (car.id == -2) {
                return crow::response(400);
            }
            auto resp = m_repo->update(car);
            if (resp.id == -1) {
                return crow::response(400);
            }
            return crow::response{"application/json", resp.toString()};
    });

    /* DELETE */
    CROW_ROUTE(app,"/car/<int>")
        .methods("DELETE"_method)([&](const crow::request& req, int id) {
            Car car;
            car.id=id;
            auto resp = m_repo->del(car);
            if (resp.id == -1) {
                return crow::response(400);
            }
            return crow::response{"application/json", resp.toString()};
    });
}