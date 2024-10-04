#pragma once
#include "model/CarRepository.h"

#include "crow_all.h"

class ServerCrow : public crow::SimpleApp {
    CarRepository *m_repo;

public:
    void setRepo(CarRepository *repo) {
        m_repo = repo;
    }

    void init();
};