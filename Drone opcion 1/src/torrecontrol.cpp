#include "torrecontrol.h"
#include "msg.h"
#include "cola.h"
#include "drone.h"

Msg mensajeLeer(0.0,0.0,(time_t)0);
std::deque<Msg> colaTemp;

void torreControl::leerMensaje(std::deque<Msg> info)
{
    colaTemp = info;
    mensajeLeer = colaTemp.get();
    cout << mensajeLeer;
}
