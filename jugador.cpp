#include "Jugador.h"
#include <algorithm>

Jugador::Jugador(int id, double cX, double cY, double rX, double rY, double rAncho, double rAlto)
    : id(id), canonX(cX), canonY(cY), anguloDisparo(45.0), velocidadDisparo(25.0),
    rivalX(rX), rivalY(rY), rivalAncho(rAncho), rivalAlto(rAlto), vivo(true) {}
Jugador::Jugador(const Jugador& otro)
    : id(otro.id), canonX(otro.canonX), canonY(otro.canonY),
    anguloDisparo(otro.anguloDisparo), velocidadDisparo(otro.velocidadDisparo),
    rivalX(otro.rivalX), rivalY(otro.rivalY),
    rivalAncho(otro.rivalAncho), rivalAlto(otro.rivalAlto), vivo(otro.vivo) {}
Jugador::~Jugador() {}
bool Jugador::recibeImpacto(double px, double py, double radio) const {
    if (!vivo) return false;
    double cx = std::min(std::max(px, rivalX), rivalX + rivalAncho);
    double cy = std::min(std::max(py, rivalY), rivalY + rivalAlto);
    double dx = px - cx;
    double dy = py - cy;
    return (dx * dx + dy * dy) <= (radio * radio);
}
void Jugador::derrotar() {
    vivo = false;
}
void Jugador::setAnguloDisparo(double angulo) {
    anguloDisparo = angulo;
}
void Jugador::setVelocidadDisparo(double velocidad) {
    velocidadDisparo = velocidad;
}
int Jugador::getId() const { return id; }
double Jugador::getCanonX() const { return canonX; }
double Jugador::getCanonY() const { return canonY; }
double Jugador::getAnguloDisparo() const { return anguloDisparo; }
double Jugador::getVelocidadDisparo() const { return velocidadDisparo; }
double Jugador::getRivalX() const { return rivalX; }
double Jugador::getRivalY() const { return rivalY; }
double Jugador::getRivalAncho() const { return rivalAncho; }
double Jugador::getRivalAlto() const { return rivalAlto; }
bool Jugador::estaVivo() const { return vivo; }