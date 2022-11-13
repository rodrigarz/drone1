#include <ctime>
#include "drone.h"
#include "cola.h"
#include "msg.h"
#include "torrecontrol.h"
#include <cmath>
#include <chrono>
#include <iostream>
#include "randomgenerator.h"

/*
    Llamadas a todas las clases necesarias para el funcionamiento del programa.
    Llamamos a la clase cola lim, y creamos una llamada vec y otra llamada logs
    Llamamos al constructor de la clase torre de control, para poder acceder a sus métodos mas tarde
    Creamos las secuencias de numeros aleatorios necesarias
    Creamos dos estructuras que se usaran en varias funciones
*/
colaLim<Msg> vec(10);
colaLim<Msg> logs(1);
torreControl torre;
RandReal BatAleatorio(0.5, 2.0);
RandReal BatArranque(0, 1);
RandReal VelSubida(0.5,1.5);
RandReal VelBajada(1,1.5);
RandReal BatEspera(-1, 1);
RandReal AltCambio(-0.5, 0.5);
CoordenadasGPS actuales, destino;


//Damos nombre al fichero, fuera de las funciones, para que todas puedan accdeder a el
std::ofstream miFichero ("droneinfo.txt");

void DroneInfo::arrancar()
{
    //Comprobamos que podemos abrir el fichero (se hace en todas las funciones)
    if(!miFichero)
    {
        cout << "Error al abrir el fichero" << endl;
        exit (1);
    }

    //Enviamos al fichero el instante en el que se dio la orden, para guardar un registro (se hace en tpdas las funciones)
    time_t inicial = mTimeStamp;
    miFichero << "Se arrancó el drone en el instante " << inicial <<endl;

    //Pedimos al usuario que nos indique las coordenadas de origen del drone, comprobando que sean reales
    do
    {
        std::cout << "Introduzca las coordenadas de actuales, latitud (intervalo +-90) y longuitud (intervalo +-180), en grados: " << std::endl;
        std::cin >> actuales.latitud;
        std::cin >> actuales.longuitud;
    }
    while ( (actuales.latitud > 90) || (actuales.latitud < -90) || (actuales.longuitud < -180) || (actuales.longuitud > 180));
    std::cout << "Latitud actual: " << actuales.latitud << ", longuitud actual: " << actuales.longuitud <<endl;

    //durante el tiempo que dura el arranque, realizamos el bucle
    for(int i=0; i<TIEMPO_ARRANQUE; i++)
    {
        //Tomamos el timepo, reducimos la bateria, y almacenamos la informacion en las dos colas, mandamos a la torre de control leer la informacion, y esperamos un segundo
        time(&mTimeStamp);
        mBattery -= BatArranque();
        vec.push(Msg(mHeight, mBattery, (time_t)mTimeStamp));
        logs.registro((Msg(mHeight, mBattery, (time_t)mTimeStamp)), 0);
        torre.leerMensaje(vec);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }


}


void DroneInfo::subirHasta(double altura, double &t)
{
    //Usamos la funcion clock para almacenar en un entero largo el instante de tiempo donde se ordeno ejecutar la accion (se hace en todas las funciones de la clase)
    long double inicio = clock();
    if(!miFichero)
    {
        cout << "Error al abrir el fichero" << endl;
        exit (1);
    }

    //Restamos a la bateria el porcentaje correspondiente al tiempo de espera
    mBattery -= t + BatEspera();


    //Comprobamos que la bateria no se ha agotado mientras esperabamos
    if(mBattery <= 0)
    {
        cout << "La batería se agoto mientras el drone esperaba la orden y aterrizó automaticamente" <<endl;
        return;
    }
    else
    {
        miFichero << "Se ordenó al drone subir hasta la altura " << altura << " en el instante " << inicio << endl;


        //Comprobamos que la altura solicitada tiene sentido
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

            //Repetimos el bucle siempre que estemos por debajo de la altura indicada
            while(mHeight<altura)
            {

                //Subimos la altura en funcion de la velocidad, gastamos bateria, almacenamos el tiempo, mandamos ambos mensajes a las colas, esperamos y sacamos la info por pantalla
                mHeight += VelSubida();
                mBattery -= BatAleatorio();
                time(&mTimeStamp);
                vec.push(Msg(mHeight, mBattery, (time_t)mTimeStamp));
                logs.registro((Msg(mHeight, mBattery, (time_t)mTimeStamp)), 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //espera un segundo. No hace nada.
                torre.leerMensaje(vec);
            }
        }
    }
}


void DroneInfo::bajarHasta(double altura, double &t)
{
    long double inicio = clock();

    if(!miFichero)
    {
        cout << "Error al abrir el fichero" << endl;
        exit (1);
    }

    mBattery -= t + BatEspera();

    if(mBattery <= 0)
    {
        cout << "La batería se agoto mientras el drone esperaba la orden y aterrizó automaticamente" <<endl;
        return;
    }
    else
    {

        //Comprobamos que la altura introducida tiene sentido para bajar
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
            //Almacenamos en el fichero la informacion con la altura indicada y el tiempo en el que se realizo la orden
            miFichero << "Se ordeno al drone bajar hasta la altura "<< altura << " en el instante " << inicio << endl;

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

                // En cada iteración, con el dato ya obtenido, comprobamos que ninguno de los dos sea negativo, ya que con los numeros
                //aleatorios que utilizmos podria llegar a ocurrir
                if( (mHeight < 0) || (mBattery < 0))
                    return;

                vec.push(Msg(mHeight, mBattery,(time_t)mTimeStamp));
                logs.registro((Msg(mHeight, mBattery,(time_t)mTimeStamp)), 0);
                torre.leerMensaje(vec);

                std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //espera un segundo. No hace nada.
            }
        }
    }
}


void DroneInfo::aterrizar(double &t)
{
    long double inicio = clock();

    if(!miFichero)
    {
        cout << "Error al abrir el fichero" << endl;
        exit (1);
    }

    //Usamos la el parametro t que le pasamos a la funcion como variable auxiliar para el caso en el que la bateria se ha agotado, y el drone tiene que aterrizar de forma autonoma
    if (t == 0)
    {
        miFichero << "Se ordenó al drone aterrizar automaticamente porque la batería se estaba agotando " <<endl;
        return;
    }

    //Si el parametro que le pasamos no es 0, indicamos en el fichero el instante en el que solicito la accion
    else
    {
        miFichero << "Se ordenó al drone aterrizar en el instante " << inicio <<endl;
    }


    mBattery -= t + BatEspera();
    double aux = 0;

    std::cout << "Aterrizando..." << std::endl;
    std::cout << "\n" << std::endl;

    //Para el aterrizaje utilizamos la funcion bajar hasta, indicandole que la altura es 0
    bajarHasta(0, aux);

    std::cout <<"Completado" << std::endl;

    //Una vez hemos aterrizado, volcamos en el fichero toda la cola logs, que es de tamaño ilimitado, para poder almacenar toda la informacion del vuelo
    miFichero << endl;
    miFichero << "Registro de la actividad: " <<endl;
    do
    {
        Msg temporal(0, 0, 0, 0, (time_t)0);
        temporal = logs.get();
        miFichero << temporal <<endl;
    }
    while(!logs.empty());

    if (mBattery <= 1)
        exit (EXIT_FAILURE);
}


//Esta funcion se ejecuta a la par con la funcion moverHasta, ya que est ultima recurre a obtenerDistancia para obtener la distancia que se recorrera
double DroneInfo::obtenerDistancia(double di)
{
    //Por seguridad, impedimos que se pueda mover el drone si la altura es inferor a 2
    if (mHeight <= 2)
    {
        cout << "ATENCION: La altura es demasiado baja para mover el drone, suba hasta una altura mayor" <<endl;
        return 0;
    }

    //Declaramos las estructuras de coordenadas que utilizaremos
    CoordenadasGPS  origenRad, destinoRad;

    double t0 = clock();


    //Solicitamos al usuario que indique las coordeadas de destino del drone, comprobando que sean reales
    do
    {
        std::cout << "Introduzca las coordenadas de destino, latitud (intervalo +-90) y longuitud (intervalo +-180), en grados: " << std::endl;
        std::cin >> destino.latitud;
        std::cin >> destino.longuitud;
    }
    while ( (destino.latitud > 90) || (destino.latitud < -90) || (destino.longuitud < -180) || (destino.longuitud > 180));
    std::cout << "Latitud de destino: " << destino.latitud << ", longuitud de destino: " << destino.longuitud <<endl <<endl;

    //Transformamos las coordenadas en grados a radianes para poder utilizar la ecuacion
    origenRad.latitud = actuales.latitud * M_PI/180;
    origenRad.longuitud = actuales.longuitud * M_PI/180;
    destinoRad.latitud = destino.latitud * M_PI/180;
    destinoRad.longuitud = destino.longuitud * M_PI/180;


    //Variables auxiliares para simplificar el cálculo, se utiliza la formula de Haversine
    double a = pow(sin( (destinoRad.latitud - origenRad.latitud)/2),2);
    double b = pow(sin((destinoRad.longuitud - origenRad.longuitud)/2),2);
    double c = (a + cos(origenRad.latitud)*cos(destinoRad.latitud)*b);

    //Calculo final para obtener la distancia total
    di = 2*RADIO*asin(sqrt (c));

    //Indicamos por pantalla la distancia que se recorre
    std::cout << "La distancia que recorreremos es de " << di << " metros" <<std::endl;

    //Calculamos la bateria que se ha gastado mientras el usuario indicaba la distancia
    double t1 = clock();
    double tiempo_ejecc = ((t1-t0)/CLOCKS_PER_SEC);

    mBattery -= round(tiempo_ejecc);

    mDistancia = di;

    vec.push(Msg(mHeight, mBattery, 0, (time_t)mTimeStamp));
    logs.registro((Msg(mHeight, mBattery, 0, (time_t)mTimeStamp)), 0);

    return(di);
}


void DroneInfo::moverHasta(const &d, double &t)
{
    long double inicio = clock();

    if(!miFichero)
    {
        cout << "Error al abrir el fichero" << endl;
        exit (1);
    }

    mBattery -= t + BatEspera();

    if(mBattery <= 0)
    {
        cout << "La batería se agoto mientras el drone esperaba la orden y aterrizó automaticamente" <<endl;
        return;
    }
    else
    {

        //Se guarda en el fichero las coordenadas de origen, las de destino, y el instante en el que se ejcuto la orden
        miFichero << "Se ordeno al drone desplazarse hasta las coordenadas " << destino.latitud << "º, " << destino.longuitud << "º desde las coordenadas " << actuales.latitud << "º, " << actuales.longuitud << "º en el instante " << inicio <<endl;

        //Inicializamos las variables que indican la distancia recorrida y la velocidad instantanea
        double recorrido = 0;
        int veloc = 0;
        std::cout << "Coordenadas de origen: Latitud; " << actuales.latitud << ", longuitud; " << actuales.longuitud <<endl;


        /*
            Mientras la distancia recorrida se menor que la indicada, realizamos las ordenes indicadas a continuacion
            En cada iteración, al principio, comprobamos que la bateria es suficiente para aterrizar en emergencia, si no se cumple, mandamos aterrizar (la bateria se gasta aproximadamente 1% por cada metro de descenso
            Si hay bateria suficiente, restamos porcentaje a la bateria, ponemos una velocidad en el rango 26 - 15, aumentamos el recorrido y simulamos pequeños cambios en la altura, para simular turbulencias
            Almacenamos los datos en ambas colas, mandamos leer la cola vec, y esperamos un segundo para repetir la iteracion
        */
        while(d >= recorrido)
        {
            if (mBattery <= mHeight)
            {
                double aux = 0;
                std::cout << "La batería se esta agotando, aterrizando automaticamente" << std::endl;
                bajarHasta(0,aux);
                aterrizar(aux);
                std::cout << "Batería agotada " <<endl;
                return;
            }

            double BatCae = BatAleatorio();
            veloc = 15+rand()%(26-15);
            recorrido = recorrido +veloc;
            mBattery -= BatCae;
            double altMoment = mHeight + AltCambio();
            time(&mTimeStamp);
            vec.push(Msg(altMoment, mBattery, recorrido, veloc, (time_t)mTimeStamp));
            logs.registro((Msg(altMoment, mBattery, recorrido, veloc, (time_t)mTimeStamp)), 0);
            torre.leerMensaje(vec);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        }

        //Cuando llegamos al destino, cambiamos las coordenadas actuales por las de destino, para que si se quiere volver a mover el drone, partamos de estas ultimas
        destino.latitud = actuales.latitud;
        destino.longuitud = actuales.longuitud;

    }

}
