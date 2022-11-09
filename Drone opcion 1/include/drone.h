#ifndef DRONE_H
#define DRONE_H


const int TIEMPO_ARRANQUE = 2;  //tiempo en segundos que emplea en arranque


struct CoordenadasGPS
{
    double latitud;
    double longuitud;
};

class DroneInfo
{
    double mHeight{0.0};
    int mBattery{100};
    time_t mTimeStamp{0};
    double mDistancia {0};
public:
   // friend class torreControl;
    void arrancar();
    void subirHasta(double h);     //orden de subir hasta h metros
    void bajarHasta(double h);     //orden de bajar hasta h metros
    void aterrizar();
    double obtenerDistancia(double di);
    void moverHasta(const &d);


};
#endif // DRONE_H
