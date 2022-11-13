#include "msg.h"


//Sobrecargamos el operador << para mostrar por pantalla la informacion en tiempo real del drone9
ostream& operator<< (ostream &out, Msg &msg)
{
    out <<"Altura: " << msg.mMsgAltura << "  Bateria:  " << msg.mMsgBateria << "  Velocidad instantanea: " << msg.mMsgRecorr << "   Distancia recorrida: " << msg.mMsgDist << "  Time Stamp:  " << msg.mMsgTime << "\n";
    return out;
}
