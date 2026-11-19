#ifndef OBSTACULO_H
#define OBSTACULO_H

class Obstaculo {
private:
    double ejex;
    double ejey;
    double ancho;
    double alto;
    double rebote;
    double resistencia;
    int dueno;
    bool destruido;


public:
    // Constructor principal
    Obstaculo(double x, double y, double an, double al, double elastico, double res, int jugadorDueno);
    Obstaculo(const Obstaculo& otro);
    ~Obstaculo();
    // Verifica si un punto cae dentro del rectangulo
   bool contienePunto(double puntox, double puntoy) const;

    // donde golpea
   int LadoColision(double puntox, double puntoy, double r) const;

   //restar resistencias
   void recibirDanio(double danio);

    // Getters
   double get_ejex() const;
   double get_ejey() const;
   double get_Ancho() const;
   double get_Alto() const;
   double get_elastico() const;
   double get_resistencia() const;
   int get_dueno() const;
   bool estaDestruido() const;
};

#endif // OBSTACULO_H