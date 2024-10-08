#include "model/CarMemRepository.h"
#include "model/CarPgSqlRepository.h"
#include "server_crow/ServerCrow.h"

int main() {
    using namespace std;

    CarPgSqlRepository pgrepo = CarPgSqlRepository();
    CarMemRepository memrepo("cars.json");
    ServerCrow app(USE_PGSQL_REPO 
                    ? dynamic_cast<CarRepository&>(pgrepo) 
                    : dynamic_cast<CarRepository&>(memrepo));

    if (USE_PGSQL_REPO) {
        pgrepo.configureConnection();
    }
    else {
        memrepo.readFromDisk();
    }

    app.init();
    app.port(18080).multithreaded().run();

    if (not USE_PGSQL_REPO)
        memrepo.saveToDisk();

    return 0;
}