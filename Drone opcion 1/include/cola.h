#ifndef COLA_H
#define COLA_H
#include <deque>
#include <ctime>
#include <thread>
#include <cmath>
#include <iostream>
#include "msg.h"
#include "drone.h"


//Establecemos el valor que se devolverá en caso que la cola este vacia
const int colaVacia = -1;

template <typename T>
class colaLim : public std::deque<T> //Cola de longuitud limitada, hereda de forma publica de deque
{
    int tamMax = 10;
    deque<T> mCola;
public:
    colaLim (int nuevoTam) {} //definimos el constructor de la clase
    void push(Msg dato); //Introducir valor en la cola, recive valor
    T get(); //obtener valor de la cola, devuelve valor
    void registro(Msg info, int a);
    int size() //definimos los metodos simples, para saber el tamaño de cola, o si esta vacia
    {
        return mCola.size();
    }
    bool empty()
    {
        return mCola.empty();
    }
};

template <typename T>
void colaLim<T>::push(Msg info)
{
    if (mCola.size() >= tamMax) //Preguntamos si la cola esta llena, y en tal caso, eliminamos el valor final, e introducimos el nuevo, solo para la cola vec
        mCola.pop_back();

    mCola.push_front(info);
}

//Definimos la funcion que nos permite obtener el valor de la cola para su lectura
template <typename T>
T colaLim<T>::get()
{
    Msg dato(0, 0, 0, 0, (time_t)0);
    //Comprobamos que la cola no esta vacia
    if (!mCola.empty())
    {
        dato = mCola.back();
        mCola.pop_back();
    }
    else
        throw colaVacia;

    return dato;

}

//Sobrecargamos la funcion push_front, indicando un valor auxiliar (en este caso un entero) para diferenciar entre las funciones
//En este caso, no eliminamos valores, ya que la cola logs es de longuitud ilimitada
template <typename T>
void colaLim<T>::registro(Msg info, int a)
{
    mCola.push_front(info);
}

#endif // COLA_H
