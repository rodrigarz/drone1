#ifndef TORRECONTROL_H
#define TORRECONTROL_H
#include <deque>
#include "drone.h"
#include "cola.h"
#include "msg.h"
#include <iostream>

//La torre de control se encarga de mandar leer los mensajes de la cola vec, que es la limitada
class torreControl
{

public:
    torreControl() {}
    void leerMensaje(colaLim<Msg>& info)
    {
        Msg mensajeLeer(0.0,0.0,(time_t)0);
        mensajeLeer = info.get();
        cout << mensajeLeer;
        return;
    }

};

#endif // TORRECONTROL_H
