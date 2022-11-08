#ifndef TORRECONTROL_H
#define TORRECONTROL_H
#include <ctime>
#include <deque>
#include <stdlib.h>
#include "drone.h"
#include "cola.h"
#include "msg.h"
#include <thread>
#include <cmath>
#include <chrono>
#include <iostream>

class torreControl
{
   //int tamMax = 10;
public:
    torreControl(){}
    void leerMensaje(std::deque<Msg> info);

};

#endif // TORRECONTROL_H
