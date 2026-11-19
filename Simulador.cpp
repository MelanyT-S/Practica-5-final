#include "Simulador.h"
#include <algorithm>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Simulador::Simulador()
    : cfg(), proyectilActivo(nullptr), jugador1(nullptr), jugador2(nullptr),
    turnoActual(1),
    ganador(0),
    disparoEnCurso(false),
    tiempoTurnoActual(0.0),
    impactosBloqueTurno(0) {
    construirEscenarioInicial();
}
Simulador::Simulador(const configuracion& config)
    : cfg(config),
    proyectilActivo(nullptr),
    jugador1(nullptr),
    jugador2(nullptr),
    turnoActual(1),
    ganador(0),
    disparoEnCurso(false),
    tiempoTurnoActual(0.0),
    impactosBloqueTurno(0) {
    construirEscenarioInicial();
}

Simulador::Simulador(const Simulador& otro)
    : cfg(otro.cfg),
    proyectilActivo(nullptr),
    jugador1(nullptr),
    jugador2(nullptr),
    turnoActual(otro.turnoActual),
    ganador(otro.ganador),
    disparoEnCurso(otro.disparoEnCurso),
    tiempoTurnoActual(otro.tiempoTurnoActual),
    impactosBloqueTurno(otro.impactosBloqueTurno) {
    if (otro.proyectilActivo) {
        proyectilActivo = new Particula(*otro.proyectilActivo);
    }
    if (otro.jugador1) {
        jugador1 = new Jugador(*otro.jugador1);
    }
    if (otro.jugador2) {
        jugador2 = new Jugador(*otro.jugador2);
    }
    for (size_t i = 0; i < otro.obstaculos.size(); ++i) {
        obstaculos.push_back(new Obstaculo(*otro.obstaculos[i]));
    }
}
Simulador& Simulador::operator=(const Simulador& otro) {
    if (this != &otro) {
        limpiarMemoria();
        cfg = otro.cfg;
        turnoActual = otro.turnoActual;
        ganador = otro.ganador;
        disparoEnCurso = otro.disparoEnCurso;
        tiempoTurnoActual = otro.tiempoTurnoActual;
        impactosBloqueTurno = otro.impactosBloqueTurno;

        if (otro.proyectilActivo) proyectilActivo = new Particula(*otro.proyectilActivo);
        if (otro.jugador1) jugador1 = new Jugador(*otro.jugador1);
        if (otro.jugador2) jugador2 = new Jugador(*otro.jugador2);
        for (size_t i = 0; i < otro.obstaculos.size(); ++i) {
            obstaculos.push_back(new Obstaculo(*otro.obstaculos[i]));
        }
    }
    return *this;
}

Simulador::~Simulador() {
    limpiarMemoria();
}

void Simulador::limpiarMemoria() {
    delete proyectilActivo;
    proyectilActivo = nullptr;

    delete jugador1;
    jugador1 = nullptr;

    delete jugador2;
    jugador2 = nullptr;

    for (size_t i = 0; i < obstaculos.size(); ++i) {
        delete obstaculos[i];
    }
    obstaculos.clear();
}

void Simulador::construirEscenarioInicial() {
    limpiarMemoria();

    double e = cfg.getRestitucionBloque();
    double resCol = cfg.getResistenciaColumna();
    double resTech = cfg.getResistenciaTecho();

    // Base Jugador 1
    obstaculos.push_back(new Obstaculo(8.0,  0.0,  6.0, 16.0, e, resCol, 1));  // Columna izq (200)
    obstaculos.push_back(new Obstaculo(26.0, 0.0,  6.0, 16.0, e, resCol, 1));  // Columna der (200)
    obstaculos.push_back(new Obstaculo(8.0,  16.0, 24.0, 6.0, e, resTech, 1)); // Techo (100)

    // Base Jugador 2
    obstaculos.push_back(new Obstaculo(68.0, 0.0,  6.0, 16.0, e, resCol, 2));  // Columna izq (200)
    obstaculos.push_back(new Obstaculo(86.0, 0.0,  6.0, 16.0, e, resCol, 2));  // Columna der (200)
    obstaculos.push_back(new Obstaculo(68.0, 16.0, 24.0, 6.0, e, resTech, 2)); // Techo (100)

    // cañones
    jugador1 = new Jugador(1, 4.0,  34.0, 17.5, 0.0, 5.0, 10.0);
    jugador2 = new Jugador(2, 96.0, 34.0, 77.5, 0.0, 5.0, 10.0);
}

void Simulador::reiniciarPartida() {
    turnoActual = 1;
    ganador = 0;
    disparoEnCurso = false;
    tiempoTurnoActual = 0.0;
    impactosBloqueTurno = 0;
    construirEscenarioInicial();
}

bool Simulador::disparar(double anguloGrados, double velocidadInicial) {
    if (disparoEnCurso || ganador != 0) return false;
    if (anguloGrados < 0.0 || anguloGrados > 90.0) return false;
    double v0 = std::min(std::max(velocidadInicial, cfg.getVelocidadMin()), cfg.getVelocidadMax());
    double rad = anguloGrados * M_PI / 180.0;
    Jugador* tirador = (turnoActual == 1) ? jugador1 : jugador2;
    tirador->setAnguloDisparo(anguloGrados);
    tirador->setVelocidadDisparo(v0);
    double vx = v0 * std::cos(rad);
    double vy = v0 * std::sin(rad);

    // El Jugador 2
    if (turnoActual == 2 && vx > 0.0) {
        vx = -vx;
    }
    delete proyectilActivo;
    proyectilActivo = new Particula(
        turnoActual,
        tirador->getCanonX(),
        tirador->getCanonY(),
        vx,
        vy,
        cfg.getMasaProyectil(),
        cfg.getRadioProyectil()
        );
    disparoEnCurso = true;
    tiempoTurnoActual = 0.0;
    impactosBloqueTurno = 0;
    return true;
}
bool Simulador::colisionarObstaculo(Particula* p, Obstaculo* o) {
    if (o->estaDestruido()) return false;
    double ox = o->get_ejex(), oy = o->get_ejey();
    double an = o->get_Ancho(), al = o->get_Alto();
    double px = p->get_ejex(), py = p->get_ejey(), r = p->getRadio();
    double cx = std::min(std::max(px, ox), ox + an);
    double cy = std::min(std::max(py, oy), oy + al);
    double dx = px - cx, dy = py - cy;
    if (dx * dx + dy * dy >= r * r) return false;
    double nx = 0.0, ny = 0.0;
    if (!o->contienePunto(px, py)) {
        if (std::fabs(dx) >= std::fabs(dy)) nx = (dx < 0) ? -1.0 : 1.0;
        else ny = (dy < 0) ? -1.0 : 1.0;
    } else {
        double izq = px - ox, der = ox + an - px;
        double aba = py - oy, arr = oy + al - py;
        double m = std::min(std::min(izq, der), std::min(aba, arr));
        if (m == izq) nx = -1.0;
        else if (m == der) nx = 1.0;
        else if (m == aba) ny = -1.0;
        else ny = 1.0;
    }
    if (nx != 0.0) p->set_ejex((nx > 0 ? ox + an : ox) + nx * r);
    if (ny != 0.0) p->set_ejey((ny > 0 ? oy + al : oy) + ny * r);
    double vn = p->getVelocidadx() * nx + p->getVelocidady() * ny;
    if (vn >= 0.0) return false;
    // Daño proporcional al momento lineal
    bool aplicarDanio = (o->get_dueno() != turnoActual) || cfg.getDanioFuegoAmigo();
    if (aplicarDanio) {
        double momento = p->getMasa() * p->getRapidez();
        double danio = cfg.getFactorDanio() * momento;
        o->recibirDanio(danio);
    }
    // Rebote inelastico
    double e = o->get_elastico();
    p->setVelocidadx(p->getVelocidadx() - (1.0 + e) * vn * nx);
    p->setVelocidady(p->getVelocidady() - (1.0 + e) * vn * ny);
    return true;
}
void Simulador::avanzarPaso() {
    if (!disparoEnCurso || !proyectilActivo || ganador != 0) return;
    double dt = cfg.getDtFisico();
    proyectilActivo->mover(dt, cfg.getGravedad());
    tiempoTurnoActual += dt;
    // Rebote elastico contra las paredes
    proyectilActivo->rebotarPared(cfg.getAncho(), cfg.getAlto());
    // Impacto contra el representante enemigo
    Jugador* enemigo = (turnoActual == 1) ? jugador2 : jugador1;
    if (enemigo->recibeImpacto(proyectilActivo->get_ejex(), proyectilActivo->get_ejey(), proyectilActivo->getRadio())) {
        enemigo->derrotar();
        ganador = turnoActual;
        disparoEnCurso = false;
        proyectilActivo->set_activa(false);
        return;
    }
    // Colisiones contra las barreras
    for (size_t i = 0; i < obstaculos.size(); ++i) {
        if (colisionarObstaculo(proyectilActivo, obstaculos[i])) {
            impactosBloqueTurno++;
        }
    }
    // 4. Fin de turno por tiempo, numero de impactos
    bool sinEnergia = (proyectilActivo->getRapidez() < 1.5 && proyectilActivo->get_ejey() <= proyectilActivo->getRadio() + 0.1);
    if (tiempoTurnoActual >= cfg.getTiempoMaxTurno() ||
        impactosBloqueTurno >= cfg.getMaxImpactosBloque() ||
        sinEnergia) {
        finalizarTurno();
    }
}
void Simulador::finalizarTurno() {
    disparoEnCurso = false;
    if (proyectilActivo) {
        proyectilActivo->set_activa(false);
    }
    turnoActual = (turnoActual == 1) ? 2 : 1;
}
//geters
const configuracion& Simulador::getConfiguracion() const { return cfg; }
const Particula* Simulador::getProyectil() const { return proyectilActivo; }
const std::vector<Obstaculo*>& Simulador::getObstaculos() const { return obstaculos; }
const Jugador* Simulador::getJugador1() const { return jugador1; }
const Jugador* Simulador::getJugador2() const { return jugador2; }
int Simulador::getTurnoActual() const { return turnoActual; }
int Simulador::getGanador() const { return ganador; }
bool Simulador::hayDisparoEnCurso() const { return disparoEnCurso; }