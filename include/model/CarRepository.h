#pragma once

#include "IRepository.h"
#include "Car.h"

struct CarRepository : virtual public IRepository<Car> {
    virtual uint getCount() const = 0;
    virtual int  getNextID() = 0;
};