#include "configuracion.h"

configuracion::configuracion():
    ancho(100.0),
    alto(60.0),
    gravedad(9.8),
    dtFisico(0.01),
    masaProyectil(2.0),
    radioProyectil(1.2),
    velocidadMin(5.0),
    velocidadMax(45.0),
    restitucionBloque(0.55),
    factorDanio(1.5),
    danioFuegoAmigo(true),
    tiempoMaxTurno(20.0),
    maxImpactosBloque(3),
    resistenciaColumna(200.0),
    resistenciaTecho(100.0) {}

configuracion::configuracion(const configuracion& otra):
    ancho(otra.ancho),
    alto(otra.alto),
    gravedad(otra.gravedad),
    dtFisico(otra.dtFisico),
    masaProyectil(otra.masaProyectil),
    radioProyectil(otra.radioProyectil),
    velocidadMin(otra.velocidadMin),
    velocidadMax(otra.velocidadMax),
    restitucionBloque(otra.restitucionBloque),
    factorDanio(otra.factorDanio),
    danioFuegoAmigo(otra.danioFuegoAmigo),
    tiempoMaxTurno(otra.tiempoMaxTurno),
    maxImpactosBloque(otra.maxImpactosBloque),
    resistenciaColumna(otra.resistenciaColumna),
    resistenciaTecho(otra.resistenciaTecho) {}
configuracion::~configuracion() {}
configuracion& configuracion::operator=(const configuracion& otra) {
    if (this != &otra) {
        ancho = otra.ancho; alto = otra.alto; gravedad = otra.gravedad; dtFisico = otra.dtFisico;
        masaProyectil = otra.masaProyectil; radioProyectil = otra.radioProyectil;
        velocidadMin = otra.velocidadMin; velocidadMax = otra.velocidadMax;
        restitucionBloque = otra.restitucionBloque; factorDanio = otra.factorDanio;
        danioFuegoAmigo = otra.danioFuegoAmigo; tiempoMaxTurno = otra.tiempoMaxTurno;
        maxImpactosBloque = otra.maxImpactosBloque;
        resistenciaColumna = otra.resistenciaColumna; resistenciaTecho = otra.resistenciaTecho;
    }
    return *this;
}
double configuracion::getAncho() const {
    return ancho;
}
double configuracion::getAlto() const {
    return alto;
}
double configuracion::getGravedad() const {
    return gravedad;
}
double configuracion::getDtFisico() const {
    return dtFisico;
}
double configuracion::getMasaProyectil() const {
    return masaProyectil;
}
double configuracion::getRadioProyectil() const {
    return radioProyectil;
}
double configuracion::getVelocidadMin() const {
    return velocidadMin;
}
double configuracion::getVelocidadMax() const {
    return velocidadMax;
}
double configuracion::getRestitucionBloque() const {
    return restitucionBloque;
}
double configuracion::getFactorDanio() const {
    return factorDanio;
}
bool   configuracion::getDanioFuegoAmigo() const {
    return danioFuegoAmigo;
}
double configuracion::getTiempoMaxTurno() const {
    return tiempoMaxTurno;
}
int    configuracion::getMaxImpactosBloque() const {
    return maxImpactosBloque;
}
double configuracion::getResistenciaColumna() const {
    return resistenciaColumna;
}
double configuracion::getResistenciaTecho() const {
    return resistenciaTecho;
}