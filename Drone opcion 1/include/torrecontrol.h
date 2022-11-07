#ifndef TORRECONTROL_H
#define TORRECONTROL_H
#include "cola.h"
#include "msg.h"
#include "drone.h"
#include <deque>


class torreControl : DroneInfo
{
   //int tamMax = 10;
    //deque<T> mCola;
public:
    torreControl();
    void leerMensaje(colaLim<Msg> info(10));

};

#endif // TORRECONTROL_H
