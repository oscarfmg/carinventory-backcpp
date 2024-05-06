#include "server_crow/ServerCrow.h"

int main() {
    using namespace std;

    CarMemRepository repo;
    ServerCrow app;

    app.setRepo(&repo);
    app.init();

    repo.create({1,"uno","one",1,"eins"});
    repo.create({2,"dos","two",2,"zwei","deux",2});
    repo.create({3,"tres","three",3,"drei","trois",3});
    repo.create({4,"cuatro","four",4,"vier","quatre",4});

    app.port(18080).multithreaded().run();

    return 0;
}