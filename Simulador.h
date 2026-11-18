#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <vector>
#include "configuracion.h"
#include "Particula.h"
#include "Obstaculo.h"
#include "Jugador.h"

class Simulador {
private:
    configuracion cfg;
    Particula* proyectilActivo;
    std::vector<Obstaculo*> obstaculos;
    Jugador* jugador1;
    Jugador* jugador2;

    int turnoActual;           // 1 = Jugador 1, 2 = Jugador 2
    int ganador;               // 0 = En curso, 1 = Gano J1, 2 = Gano J2
    bool disparoEnCurso;       // true mientras el proyectil esta en movimiento
    double tiempoTurnoActual;  // Tiempo transcurrido del disparo actual
    int impactosBloqueTurno;   // Contador de choques contra bloques en el turno

    void limpiarMemoria();
    void construirEscenarioInicial();
    bool colisionarObstaculo(Particula* p, Obstaculo* o);
    void finalizarTurno();

public:
    Simulador();
    Simulador(const configuracion& config);
    Simulador(const Simulador& otro);
    Simulador& operator=(const Simulador& otro);
    ~Simulador();

    // Control principal del juego
    void reiniciarPartida();
    bool disparar(double anguloGrados, double velocidadInicial);
    void avanzarPaso();

    // Getters para que la interfaz grafica (Qt) consulte el estado
    const configuracion& getConfiguracion() const;
    const Particula* getProyectil() const;
    const std::vector<Obstaculo*>& getObstaculos() const;
    const Jugador* getJugador1() const;
    const Jugador* getJugador2() const;
    int getTurnoActual() const;
    int getGanador() const;
    bool hayDisparoEnCurso() const;
};

#endif//SIMULADOR_H
