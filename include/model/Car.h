#pragma once

#include <string>
#include <ostream>

struct Car {
    int id;
    std::string model;
    std::string brand;
    int kilometers;
    std::string price;
    std::string description;
    int year;
public:
    Car(int id, std::string model, std::string brand,
    int kilometers, std::string price, std::string description = "",
    int year = -1);
    Car() = default;
    Car(const Car& car) = default;

    std::string toString() const;
    friend std::ostream& operator <<(std::ostream &out, const Car& car);
};