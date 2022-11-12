#ifndef DRONE_H
#define DRONE_H


const int TIEMPO_ARRANQUE = 2;  //tiempo en segundos que emplea en arranque

//Usamos una estructura para trabajar mas facilmente con las coordenadas
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
    void arrancar();
    void subirHasta(double h, double &t);
    void bajarHasta(double h, double &t);
    void aterrizar(double &t);
    double obtenerDistancia(double di);
    void moverHasta(const &d, double &t);


};
#endif // DRONE_H
