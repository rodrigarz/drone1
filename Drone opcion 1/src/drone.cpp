#include <ctime>
#include <deque>
#include <stdlib.h>
#include "drone.h"
#include "cola.h"
#include "msg.h"
#include "torrecontrol.h"
#include <thread>
#include <cmath>
#include <chrono>
#include <iostream>



colaLim<Msg> vec(10);
torreControl torre;


void DroneInfo::arrancar()
{
    for(int i=0; i<TIEMPO_ARRANQUE; i++)
    {
        time(&mTimeStamp);
        mBattery -= 1;
        vec.push(Msg(mHeight, mBattery, (time_t)mTimeStamp));          //Ha pasado un segundo, tenemos 1% menos de bater�a
        torre.leerMensaje(vec);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void DroneInfo::subirHasta(double altura)
{
    if(altura < 0)
    {
        cout << "ERROR, no se puede ascender a una  altura negativa" <<endl;
        return;
    }
    else if (altura < mHeight)
    {
        cout << "ERROR, no se puede ascendere hacia abajo" <<endl;
        return;
    }
    else
    {
        std::cout << "Ascendiendo a "<< altura << " metros..."<< std::endl;
        std::cout << "\n" << std::endl;
        while(mHeight<altura)
        {
            mHeight += 1;
            mBattery -= 1;
            time(&mTimeStamp);
            vec.push(Msg(mHeight, mBattery, (time_t)mTimeStamp));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //espera un segundo. No hace nada.
            torre.leerMensaje(vec);
        }
    }
}

void DroneInfo::bajarHasta(double altura)
{
    if(altura < 0)
    {
        cout << "ERROR, la altura no puede ser negativa" <<endl;
        return;
    }
    else if(altura > mHeight)
    {
        cout << "ERROR, no puedes bajar hacia arriba" <<endl;
        return;
    }
    else
    {
        if(altura)
            {
            //para no imprimir "descendiento a 0 metros..." (aterrizaje)
            std::cout << "Descendiendo a "<< altura << " metros..."<< std::endl;
            std::cout << "\n" << std::endl;
            }
        while(mHeight>altura)
        {
            mHeight -= 1;
            mBattery -= 1;
            time(&mTimeStamp);
            vec.push(Msg(mHeight, mBattery,(time_t)mTimeStamp));
            torre.leerMensaje(vec);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //espera un segundo. No hace nada.
        }
    }
}

void DroneInfo::aterrizar()
{
    std::cout << "Aterrizando..." << std::endl;
    std::cout << "\n" << std::endl;
    std::cout << "Aterrizaje" <<std::endl;
    bajarHasta(0);

    std::cout <<"Completado" << std::endl;
}

double DroneInfo::obtenerDistancia(double di)
{
    CoordenadasGPS origen, destino, origenRad, destinoRad;

    double r = 6370000;
    double t0 = clock();

    std::cout << "Introduzca coordenadas de origen, latitud y longuitud, en grados: " << std::endl;
    std::cin >> origen.latitud;
    std::cin >> origen.longuitud;

    std::cout << "Introduzca coordenadas de destino, latitud y longuitud, en grados: " << std::endl;
    std::cin >> destino.latitud;
    std::cin >> destino.longuitud;

    origenRad.latitud = origen.latitud * M_PI/180;
    origenRad.longuitud = origen.longuitud * M_PI/180;
    destinoRad.latitud = destino.latitud * M_PI/180;
    destinoRad.longuitud = destino.longuitud * M_PI/180;

    di = 2*r*asin(sqrt( pow(sin( (destinoRad.latitud - origenRad.latitud)/2),2) + cos(origenRad.latitud)*cos(destinoRad.latitud)*pow(sin((destinoRad.longuitud - origenRad.longuitud)/2),2)));

    std::cout << "La distancia que recorreremos es de " << di << " metros" <<std::endl;

    double t1 = clock();
    double tiempo_ejecc = ((t1-t0)/CLOCKS_PER_SEC);

    std::cout << "Mientras escribias se ha gastado un " << round(tiempo_ejecc) << "% de la bateria" <<std::endl;

    mBattery -= round(tiempo_ejecc);

    mDistancia = di;

    vec.push(Msg(mHeight, mBattery, di, (time_t)mTimeStamp));

    return(di);
}

void DroneInfo::moverHasta(const &d)
{
    if (mHeight <= 2)
    {
        cout << "ATENCION: La altura es demasiado baja para mover el drone, suba hasta una altura mayor" <<endl;
        return;
    }
    else
    {
    double recorrido = 0;
    int veloc = 0;
    srand(time(NULL));
    while(d >= recorrido)
        {
            if (mBattery <= mHeight)
            {
                std::cout << "La bater�a se esta agotando, aterrizando automaticamente" << std::endl;
                aterrizar();
                std::cout << "Bater�a agotada " <<endl;
                break;
            }
            veloc = 15+rand()%(26-15);
            recorrido = recorrido +veloc;
            mBattery -= 1;
            time(&mTimeStamp);
            vec.push(Msg(mHeight, mBattery, recorrido, veloc, (time_t)mTimeStamp));
            torre.leerMensaje(vec);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        }
    }

}


