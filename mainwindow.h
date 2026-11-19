#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QPixmap>
#include "Simulador.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnDisparar_clicked();
    void actualizarEscena();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *escena;
    QTimer *temporizador;
    Simulador *simulador;
    QPixmap spriteCanon;

    // Dimensiones logicas de la escena en pixeles para escalar el mundo 100x60
    const double anchoEscena = 900.0;
    const double altoEscena = 540.0;

    // Funciones auxiliares para convertir coordenadas del mundo fisico a pixeles de Qt
    double mapX(double xMundo) const;
    double mapY(double yMundo) const;
    double mapAncho(double wMundo) const;
    double mapAlto(double hMundo) const;

    // Funciones auxiliares para dibujar el escenario, canones, munecos y estado de turno
    void dibujarEscenario();
    void dibujarCanon(double canonX, double canonY, double anguloGrados, bool esJugadorDerecha);
    void dibujarMunecoPalitos(double rx, double ry, double rAncho, double rAlto, bool vivo);
    void actualizarEstadoTurno();
};

#endif // MAINWINDOW_H