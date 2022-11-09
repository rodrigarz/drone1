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
#include "randomgenerator.h"


//Latitud entre -90 y 90
//Longuitud entre -180 y 180


colaLim<Msg> vec(10);
torreControl torre;
RandReal BatAleatorio(0.5, 2.0);
RandReal BatArranque(0, 1);
RandReal VelSubida(0.5,2);
RandReal VelBajada(1,2.5);
RandReal BatEspera(-1, 1);
CoordenadasGPS actuales, destino;


void DroneInfo::arrancar()
{
    do
    {
    std::cout << "Introduzca las coordenadas de actuales, latitud (intervalo +-90) y longuitud (intervalo +-180), en grados: " << std::endl;
    std::cin >> actuales.latitud;
    std::cin >> actuales.longuitud;
    }while ( (actuales.latitud > 90) || (actuales.latitud < -90) || (actuales.longuitud < -180) || (actuales.longuitud > 180));
    std::cout << "Latitud actual: " << actuales.latitud << ", longuitud actual: " << actuales.longuitud <<endl;


    for(int i=0; i<TIEMPO_ARRANQUE; i++)
    {
        time(&mTimeStamp);
        mBattery -= BatArranque();
        vec.push(Msg(mHeight, mBattery, (time_t)mTimeStamp));          //Ha pasado un segundo, tenemos 1% menos de batería
        torre.leerMensaje(vec);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}


void DroneInfo::subirHasta(double altura, double &t)
{
    mBattery -= t + BatEspera();

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
            mHeight += VelSubida();
            mBattery -= BatAleatorio();
            time(&mTimeStamp);
            vec.push(Msg(mHeight, mBattery, (time_t)mTimeStamp));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //espera un segundo. No hace nada.
            torre.leerMensaje(vec);
        }
    }
}


void DroneInfo::bajarHasta(double altura, double &t)
{
    mBattery -= t + BatEspera();

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
            mHeight -= VelBajada();
            mBattery -= BatAleatorio();
            time(&mTimeStamp);
            vec.push(Msg(mHeight, mBattery,(time_t)mTimeStamp));
            torre.leerMensaje(vec);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //espera un segundo. No hace nada.
        }
    }
}


void DroneInfo::aterrizar(double &t)
{
    mBattery -= t + BatEspera();

    double aux = 0;
    std::cout << "Aterrizando..." << std::endl;
    std::cout << "\n" << std::endl;
    std::cout << "Aterrizaje" <<std::endl;
    bajarHasta(0, aux);
    std::cout <<"Completado" << std::endl;
    if (mBattery <= 1)
        exit (EXIT_FAILURE);
}


double DroneInfo::obtenerDistancia(double di)
{
    CoordenadasGPS origen, origenRad, destinoRad;

    double r = 6370000;
    double t0 = clock();


    std::cout << "Introduzca coordenadas de destino, latitud y longuitud, en grados: " << std::endl;
    std::cin >> destino.latitud;
    std::cin >> destino.longuitud;


    origenRad.latitud = actuales.latitud * M_PI/180;
    origenRad.longuitud = actuales.longuitud * M_PI/180;
    destinoRad.latitud = destino.latitud * M_PI/180;
    destinoRad.longuitud = destino.longuitud * M_PI/180;


    //Variables auxiliares para simplificar el cálculo
    double a = pow(sin( (destinoRad.latitud - origenRad.latitud)/2),2);
    double b = pow(sin((destinoRad.longuitud - origenRad.longuitud)/2),2);
    double c = (a + cos(origenRad.latitud)*cos(destinoRad.latitud)*b);

    di = 2*r*asin(sqrt (c));


    std::cout << "La distancia que recorreremos es de " << di << " metros" <<std::endl;


    double t1 = clock();
    double tiempo_ejecc = ((t1-t0)/CLOCKS_PER_SEC);


    std::cout << "Mientras escribias se ha gastado un " << round(tiempo_ejecc) << "% de la bateria" <<std::endl;

    mBattery -= round(tiempo_ejecc);

    mDistancia = di;

    vec.push(Msg(mHeight, mBattery, 0, (time_t)mTimeStamp));

    return(di);
}


void DroneInfo::moverHasta(const &d, double &t)
{

    mBattery -= t + BatEspera();

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
        torre.leerMensaje(vec);
        std::cout << "Coordenadas de origen: Latitud; " << actuales.latitud << ", longuitud; " << actuales.longuitud <<endl;

        while(d >= recorrido)
            {
                if (mBattery <= mHeight)
                {
                    double aux = 0;
                    std::cout << "La batería se esta agotando, aterrizando automaticamente" << std::endl;
                    aterrizar(aux);
                    std::cout << "Batería agotada " <<endl;
                    break;
                }

                double BatCae = BatAleatorio();
                veloc = 15+rand()%(26-15);
                recorrido = recorrido +veloc;
                mBattery -= BatCae;
                time(&mTimeStamp);
                vec.push(Msg(mHeight, mBattery, recorrido, veloc, (time_t)mTimeStamp));
                torre.leerMensaje(vec);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            }

            destino.latitud = actuales.latitud;
            destino.longuitud = actuales.longuitud;
    }

}


