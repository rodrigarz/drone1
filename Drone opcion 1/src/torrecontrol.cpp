#include "torrecontrol.h"
#include "msg.h"
#include "cola.h"
#include "drone.h"

Msg mensajeLeer(0.0,0.0,(time_t)0);
colaLim<Msg> colaTemp(10);

void torreControl::leerMensaje(info)
{
    colaTemp = *miPunt;
    mensajeLeer = colaTemp.get();
    cout << mensajeLeer;
}
