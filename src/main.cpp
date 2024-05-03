#include <iostream>
#include "model/Car.h"
#include "model/CarMemRepository.h"

int main() {
    using namespace std;
    std::cout << "HCEX" << std::endl;
    Car car(1,"p","u",(int)'t',"o");

    cout << car << endl;

    CarMemRepository repo("QTFG.txt");

    Car car2 = repo.create({2,"p","u",'t',"o"});
    Car car3 = repo.create({3,"e","l",' ',"q","u",'e'});
    cout << car2 << endl;

    repo.saveToDisk();

    return 0;
}