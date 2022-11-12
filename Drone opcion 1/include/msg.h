#ifndef MSG_H
#define MSG_H


#include <deque>
#include <ctime>
#include <thread>
#include <cmath>
#include <iostream>
#include <fstream>


using namespace std;



class Msg //Clase mensaje
{
    double mMsgAltura{0.0};
    double mMsgBateria{0.0};
    time_t mMsgTime{0};
    double mMsgDist{0.0};
    double mMsgRecorr{0.0};
public:
    Msg(double altura, double bateria, double distancia, double recorrido, time_t tiempo)
    {
        mMsgAltura = altura;
        mMsgBateria = bateria;
        mMsgTime = tiempo;
        mMsgDist = distancia;
        mMsgRecorr = recorrido;
    }
    Msg(double altura = 0, double bateria = 0, time_t tiempo = 0)
    {
        mMsgAltura = altura;
        mMsgBateria = bateria;
        mMsgTime = tiempo;
    }
    Msg(double altura = 0, double bateria = 0, double distancia = 0, time_t tiempo = 0)
    {
        mMsgAltura = altura;
        mMsgBateria = bateria;
        mMsgDist = distancia;
        mMsgTime = tiempo;
    }
    double getAltura() {return mMsgAltura;}
    double getBateria() {return mMsgBateria;}
    time_t getTime() {return mMsgTime;}
    double getDistancia() {return mMsgDist;}
    double getRecorrido() {return mMsgRecorr;}
    friend ostream& operator<< (ostream &out, Msg &msg);
};

#endif // MSG_H
