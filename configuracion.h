#ifndef CONFIGURACION_H
#define CONFIGURACION_H

class configuracion
{
private:
    // Escenario
    double ancho;
    double alto;
    double gravedad;
    double dtFisico;
    // Proyectil
    double masaProyectil;
    double radioProyectil;
    double velocidadMin;
    double velocidadMax;
    // Colisiones y daño
    double restitucionBloque;
    double factorDanio;
    bool danioFuegoAmigo;
    // Fin de turno
    double tiempoMaxTurno;
    int maxImpactosBloque;
    // Resistencias iniciales
    double resistenciaColumna;
    double resistenciaTecho;
public:
    configuracion();
    configuracion(const configuracion& otra);
    ~configuracion();
    configuracion& operator=(const configuracion& otra);
    double getAncho() const;
    double getAlto() const;
    double getGravedad() const;
    double getDtFisico() const;
    double getMasaProyectil() const;
    double getRadioProyectil() const;
    double getVelocidadMin() const;
    double getVelocidadMax() const;
    double getRestitucionBloque() const;
    double getFactorDanio() const;
    bool   getDanioFuegoAmigo() const;
    double getTiempoMaxTurno() const;
    int    getMaxImpactosBloque() const;
    double getResistenciaColumna() const;
    double getResistenciaTecho() const;
};

#endif // CONFIGURACION_H