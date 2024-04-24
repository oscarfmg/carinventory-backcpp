#pragma once

#include <string>

class Car {
    int id;
    std::string model;
    std::string description;
    int year;
    std::string brand;
    int kilometers;
    std::string price;
    Car(int id, std::string model, std::string brand,
    int kilometers, std::string price);
};