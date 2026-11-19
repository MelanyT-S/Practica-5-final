#include "Obstaculo.h"
#include <cmath>

// Constructor del obstaculo cuadrado
Obstaculo::Obstaculo(double x, double y, double an, double al, double elastico, double res, int jugadorDueno)
    : ejex(x), ejey(y), ancho(an), alto(al), rebote(elastico),
    resistencia(res), dueno(jugadorDueno), destruido(res <= 0.0) {}

Obstaculo::Obstaculo(const Obstaculo& otro)
    : ejex(otro.ejex), ejey(otro.ejey), ancho(otro.ancho), alto(otro.alto),
    rebote(otro.rebote), resistencia(otro.resistencia),
    dueno(otro.dueno), destruido(otro.destruido) {}

Obstaculo::~Obstaculo() {}
// Comprueba si unas coordenadas caen dentro de los limites del cuadrado
bool Obstaculo::contienePunto(double puntox, double puntoy) const {
    return (puntox >= ejex && puntox <= ejex + ancho &&
            puntoy >= ejey && puntoy <= ejey + alto);
}

// Evalua por cual de las 4 caras ocurre la colision para elrebote inelastico
int Obstaculo::LadoColision(double puntox, double puntoy, double r) const {
    if (destruido) return 0;
    // 1. Encuentra el punto mas cercano del cuadrado al centro de la particula (circulo)
    double cercanoX = puntox;
    double cercanoY = puntoy;

    if (puntox < ejex) {
        cercanoX = ejex;
    } else if (puntox > ejex + ancho) {
        cercanoX = ejex + ancho;
    }

    if (puntoy < ejey) {
        cercanoY = ejey;
    } else if (puntoy > ejey + alto) {
        cercanoY = ejey + alto;
    }
    // 2. Calcula la distancia al cuadrado entre el centro y ese punto cercano
    double distX = puntox - cercanoX;
    double distY = puntoy - cercanoY;
    double distanciaSq = distX * distX + distY * distY;

    // 3. Validacion contacto
    if (distanciaSq <= r * r) {
        double dIzq = std::fabs(puntox - ejex);
        double dDer = std::fabs(puntox - (ejex + ancho));
        double dInf = std::fabs(puntoy - ejey);
        double dSup = std::fabs(puntoy - (ejey + alto));

        double minVal = dIzq;
        int ladoCol = 1; // 1: Izquierda

        if (dDer < minVal) {
            minVal = dDer;
            ladoCol = 2; // 2: Derecha
        }
        if (dInf < minVal) {
            minVal = dInf;
            ladoCol = 3; // 3: Inferior
        }
        if (dSup < minVal) {
            ladoCol = 4; // 4: Superior
        }
        return ladoCol;
    }

    return 0;
}
void Obstaculo::recibirDanio(double danio) {
    if (destruido || danio <= 0.0) return;

    resistencia -= danio;
    if (resistencia <= 0.0) {
        resistencia = 0.0;
        destruido = true;
    }
}
//geters
double Obstaculo::get_ejex() const {
    return ejex;
}
double Obstaculo::get_ejey() const {
    return ejey;
}
double Obstaculo::get_Ancho() const {
    return ancho;
}
double Obstaculo::get_Alto() const {
    return alto;
}
double Obstaculo::get_elastico() const {
    return rebote;
}
double Obstaculo::get_resistencia() const {
    return resistencia;
}
int Obstaculo::get_dueno() const {
    return dueno;
}
bool Obstaculo::estaDestruido() const {
    return destruido;
}