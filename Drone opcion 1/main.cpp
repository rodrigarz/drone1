//Falta limitar tamañoi de cola, implementar orden de desconexion y alguna cosilla más
//recomendable separar la formula de la distancia en varias operaciones distintas
//quitar salidas y mostrar cola


#include <iostream>
#include <ctime>
#include <thread>
#include <cmath>
#include <deque>
#include <chrono>
#include "drone.h"
#include "cola.h"
#include "msg.h"
#include "torrecontrol.h"
#include <fstream>

using namespace std;

int main()
{
    //Llamada a los constructores de las dos clases
    DroneInfo miDron;
    torreControl torre;

    char opcion;
    double alt = 0;
    int estado = 0; // Variable auxiliar para indicar los estados del drone, 1 para arrancado, 2 para en altura, 3 para desplazando, 4 para bajando, 5 para aterrizar y apagar



    do
    {
        //Indicaciones para el usuario
        cout << endl;
        cout << "----------------------------------------------------------------------------------------------------" <<endl;
        cout << "Indique la accion que ejecutara el drone." << endl;
        cout << "A: arrancar el drone" <<endl << "S: Subir hasta la altura indicada" << endl;
        cout << "D: Desplazarse desde las coordenadas indicadas hasta las coordenadas indicadas" <<endl;
        cout << "B: Bajar hasta la altura indicada" << endl << "Q: Aterrizar y desconectar (si el drone no esta activo, salir del programa)" <<endl;
        cout << "NOTA: Tenga en mente que mientras el drone esta esperando a una accion, la bateria se puede agotar" <<endl;
        cout << "----------------------------------------------------------------------------------------------------" << endl;


        //Medimos el timpo que se tarda en seleccionar la orden, para transformarlo en descenso del nivel de batería
        double t0 = clock();
        cin >> opcion;
        double t1 = clock();
        double tiempo_ejecc = ((t1-t0)/CLOCKS_PER_SEC);


        /*
            Usamos el switch para elegir entre las distintas ordenes implementadas, en cada una, comprobamos el estado actual del drone mediante la variable auxiliar,
            para evitar que el drone ejecute cualquier orden sin estar arrancado. Si el drone esta en disposicion de ejecutar la orden, llamamos a la funcion correspondiente
            de la clase drone, pasandole como argumento los parametros requeridos, junto al timepo que se ha tardado en mandar el comando.
        */
        switch(opcion)
        {
        case 'A':
            if(estado != 0)
            {
                cout << "Error, el drone ya esta activo" <<endl;
                cout << endl;
                break;
            }
            else
            {
                estado = 1;
                miDron.arrancar();
                break;
            }
        case 'S':
            if(estado != 0)
            {
                estado = 2;
                cout << "Introduzca la altura deseada: ";
                cin >> alt;
                miDron.subirHasta(alt, tiempo_ejecc);
                break;
            }
            else
            {
                cout << "ERROR, antes de ejecutar una accion debes arrancar el drone" <<endl;
                break;
            }
        case 'D':
            if(estado != 0)
            {
                estado = 3;
                miDron.moverHasta(miDron.obtenerDistancia(0), tiempo_ejecc);
                break;
            }
            else
            {
                cout << "ERROR, antes de ejecutar una accion debes arrancar el drone" <<endl;
                break;
            }
        case 'B':
            if(estado != 0)
            {
                estado = 4;
                cout << "Introduzca la altura deseada: ";
                cin >> alt;
                miDron.bajarHasta(alt, tiempo_ejecc);
                break;
            }
            else
            {

                cout << "ERROR, antes de ejecutar una accion debes arrancar el drone" <<endl;
                break;
            }
        case 'Q':
            if(estado != 0)
            {
                estado = 5;
                miDron.aterrizar(tiempo_ejecc);
                break;
            }
            else
            {
                cout << "Saliendo del programa..." <<endl;
                return 1;
            }
        default: //Si la orden introducida no se reconoce, se pide que se vuelva a introducir
            cout << "ERROR: No se ha reconocido el comando introducido, pruebe de nuevo" <<endl;
        }
    }
    while (opcion != 'Q');  //En el momento en el que la opcion es Q, salimos del bucle y se para el programa

    return 0;
}

