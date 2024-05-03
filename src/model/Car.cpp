#include "model/Car.h"
#include "fmt/format.h"

Car::Car(int id, std::string model, std::string brand,
    int kilometers, std::string price, std::string description, int year)
    : id(id)
    , model(model)
    , brand(brand)
    , kilometers(kilometers)
    , price(price)
    , description(description)
    , year(year)
{
}

std::string Car::toString() const {
    return fmt::format(
        "{{"
            "\"id\":{id},"
            "\"model\":\"{model}\","
            "\"brand\":\"{brand}\","
            "\"kilometers\":{kilometers},"
            "\"price\":\"{price}\","
            "\"description\":\"{description}\","
            "\"year\":{year}"
        "}}",
        fmt::arg("id",id),
        fmt::arg("model",model),
        fmt::arg("brand",brand),
        fmt::arg("kilometers",kilometers),
        fmt::arg("price",price),
        fmt::arg("description",description),
        fmt::arg("year",year)
    );
}

std::ostream& operator <<(std::ostream &out, const Car& car) {
    return out << car.toString();
}