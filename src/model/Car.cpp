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

std::ostream& operator <<(std::ostream &out, const Car& car) {
    std::string fmtstr = 
        "[Car] {{"
            "\"id\":{id},"
            "\"model\":\"{model}\",";
    if (car.description != "")
        fmtstr += "\"description\":\"{description}\",";
    if (car.year > 0)
        fmtstr += "\"year\":{year},";
    fmtstr +=
            "\"brand\":\"{brand}\","
            "\"kilometers\":{kilometers},"
            "\"price\":\"{price}\""
        "}}";

    return out << fmt::format(fmt::runtime(fmtstr.c_str()),
        fmt::arg("id",car.id),
        fmt::arg("model",car.model),
        fmt::arg("description",car.description),
        fmt::arg("year",car.year),
        fmt::arg("brand",car.brand),
        fmt::arg("kilometers",car.kilometers),
        fmt::arg("price",car.price)
    );

    return out;
}