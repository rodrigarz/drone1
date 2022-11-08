#ifndef COLA_H
#define COLA_H
#include <deque>
#include <ctime>
#include <thread>
#include <cmath>
#include <iostream>
#include "msg.h"
#include "drone.h"



const int colaVacia = -1;

template <typename T>
class colaLim : public std::deque<T> //Cola de longuitud limitada
{
    int tamMax = 10;
    deque<T> mCola;
public:
    colaLim (int nuevoTam):tamMax(10) {} //Es igual que poner que el nuevo_tam es igual a tam_max, con el cuerpo del constructor vacio
    void push(Msg dato); //Introducir valor en la cola, recive valor
    T get(); //obtener valor de la cola, es un valor, no recive valor
    int size()
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
    if (mCola.size() >= tamMax) //Preguntamos si la cola esta llena, y en tal caso, eliminamos el valor final, e introducimos el nuevo
        mCola.pop_back();

    mCola.push_front(info);
}

template <typename T>
T colaLim<T>::get()
{
    Msg dato(0, 0, 0, 0, (time_t)0);
    if (!mCola.empty())
    {
        dato = mCola.back();
        mCola.pop_back();
    }
    else
        throw colaVacia;

    return dato;

}
#endif // COLA_H
