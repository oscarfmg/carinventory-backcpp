#include <iostream>
#include "model/Car.h"
#include "model/CarMemRepository.h"

int main() {
    using namespace std;
    std::cout << "HCEX" << std::endl;
    Car car(1,"p","u",(int)'t',"o");

    cout << car << endl;

    CarMemRepository repo;

    Car car2 = repo.create({2,"p","u",'t',"o"});
    cout << car2 << endl;
    
    return 0;
}