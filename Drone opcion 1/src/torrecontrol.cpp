#include "torrecontrol.h"
#include "msg.h"
#include "cola.h"
#include "drone.h"

Msg mensajeLeer(0.0,0.0,(time_t)0);

void torreControl::leerMensaje(const std::deque<Msg> *miPunt)
{
    mensajeLeer = vec.get();
    cout << mensajeLeer;
}
