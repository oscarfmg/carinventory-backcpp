#include "model/Car.h"

Car::Car(int id, std::string model, std::string brand,
    int kilometers, std::string price, std::string description, int years)
    : id(id)
    , model(model)
    , brand(brand)
    , kilometers(kilometers)
    , price(price)
{
}

std::ostream& operator <<(std::ostream &out, const Car& car) {
    out << "[Car] {";
    out << car.id << ",";
    out << car.model << ",";
    if (car.description != "")
        out << car.description << ",";
    if (car.year > 0)
        out << car.year << ",";
    out << car.brand << ",";
    out << car.kilometers << ",";
    out << car.price;
    out << "}";
    return out;
}