#pragma once

#include "IRepository.h"
#include "Car.h"

struct CarRepository : public IRepository<Car> {
};