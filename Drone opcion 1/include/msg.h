#ifndef MSG_H
#define MSG_H
#include <iostream>
#include <fstream>

using namespace std;

//Definimos la clase mensaje, que será la que utilicemos para las colas
class Msg
{
    //Son los parametros que forman los mensajes
    double mMsgAltura{0.0};
    double mMsgBateria{0.0};
    time_t mMsgTime{0};
    double mMsgDist{0.0};
    double mMsgRecorr{0.0};
public:
    //Definimos tres constructores distintos, para los distintos tipos de mensajes que utilizaremos
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
    friend ostream& operator<< (ostream &out, Msg &msg);
};

#endif // MSG_H
