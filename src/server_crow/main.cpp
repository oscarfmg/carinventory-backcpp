#include "model/CarMemRepository.h"
#include "model/CarPgSqlRepository.h"
#include "server_crow/ServerCrow.h"

int main() {
    using namespace std;

    //CarMemRepository repo("cars.json");
    CarPgSqlRepository repo;
    repo.configureConnection();

    ServerCrow app;

    app.setRepo(&repo);
    app.init();

//    repo.readFromDisk();
    app.port(18080).multithreaded().run();

//    repo.saveToDisk();

    return 0;
}