//Falta limitar tamañoi de cola, implementar orden de desconexion y alguna cosilla más
//recomendable separar la formula de la distancia en varias operaciones distintas
//quitar salidas y mostrar cola


#include <iostream>
#include <ctime>
#include <thread>
#include <cmath>
#include <deque>
#include "drone.h"
#include "cola.h"
#include "msg.h"
#include "torrecontrol.h"

int main()
{
    DroneInfo miDron;
    std::deque<DroneInfo> bufferTorreControl;

    miDron.arrancar();
    miDron.subirHasta(10);
    miDron.moverHasta(miDron.obtenerDistancia(0));
    miDron.bajarHasta(7);
    miDron.aterrizar();

    return 0;
}

