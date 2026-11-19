#ifndef JUGADOR_H
#define JUGADOR_H

class Jugador {
private:
    int id;// 1 = Jugador 1 (Izquierda), 2 = Jugador 2 (Derecha)
    // Posicion de la boca del canon
    double canonX;
    double canonY;
    // Parametros del ultimo lanzamiento configurado
    double anguloDisparo;
    double velocidadDisparo;
    // Caja de colision del representante (Rival)
    double rivalX; // Esquina inferior izquierda X
    double rivalY; // Esquina inferior izquierda Y
    double rivalAncho;
    double rivalAlto;
    bool vivo;

public:
    Jugador(int id, double cX, double cY, double rX, double rY, double rAncho, double rAlto);
    Jugador(const Jugador& otro);
    ~Jugador();

    // Verifica si el proyectil circular impacto al Rival
    bool recibeImpacto(double px, double py, double radio) const;
    void derrotar();
    // Setters para registrar el apuntado del turno
    void setAnguloDisparo(double angulo);
    void setVelocidadDisparo(double velocidad);
    // Getters
    int getId() const;
    double getCanonX() const;
    double getCanonY() const;
    double getAnguloDisparo() const;
    double getVelocidadDisparo() const;
    double getRivalX() const;
    double getRivalY() const;
    double getRivalAncho() const;
    double getRivalAlto() const;
    bool estaVivo() const;
};
#endif // JUGADOR_H
