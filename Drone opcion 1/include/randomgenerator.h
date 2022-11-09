#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <iostream>
#include <ctime>
#include <random>

using namespace std;

class RandReal
{
    default_random_engine re {static_cast<long unsigned int>(time(0))};
    uniform_real_distribution<double> dist;
    //normal_distribution<double> dist;
public:
    RandReal(double minimo, double maximo):dist(minimo,maximo) {}
    double operator()() {return dist(re);}
};

#endif // RANDOMGENERATOR_H
