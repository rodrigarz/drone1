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

public:
    torreControl(){}
    void leerMensaje(colaLim<Msg>& info)
    {
    Msg mensajeLeer(0.0,0.0,(time_t)0);
    mensajeLeer = info.get();
    cout << mensajeLeer;
    return;
    }

};

#endif // TORRECONTROL_H
