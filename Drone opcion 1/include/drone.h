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
    double mBattery{100.0};
    time_t mTimeStamp{0};
    double mDistancia {0};
public:
   // friend class torreControl;
    void arrancar();
    void subirHasta(double h, double &t);     //orden de subir hasta h metros
    void bajarHasta(double h, double &t);     //orden de bajar hasta h metros
    void aterrizar(double &t);
    double obtenerDistancia(double di);
    void moverHasta(const &d, double &t);


};
#endif // DRONE_H
