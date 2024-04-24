#include "model/Car.h"

Car::Car(int id, std::string model, std::string brand,
    int kilometers, std::string price)
    : id(id)
    , model(model)
    , brand(brand)
    , kilometers(kilometers)
    , price(price)
{
}