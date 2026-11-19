#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QImage>
#include <QRgb>
#include <QPen>
#include <QBrush>
#include <cmath>
#include <queue>
#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Crear la escena grafica y asignarla al QGraphicsView
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, anchoEscena, altoEscena);
    ui->graphicsView->setScene(escena);

    // Instanciar el simulador fisico de la partida
    simulador = new Simulador();

    // Cargar el sprite del canon desde el directorio de trabajo
    spriteCanon.load("canon.png");

    // Configurar el temporizador para avanzar el paso fisico y redibujar la escena
    temporizador = new QTimer(this);
    int intervaloMs = static_cast<int>(simulador->getConfiguracion().getDtFisico() * 1000.0);
    if (intervaloMs < 5) {
        intervaloMs = 10; // Evita un intervalo demasiado pequeno en el temporizador
    }
    temporizador->setInterval(intervaloMs); // Asigna el intervalo calculado al temporizador
    connect(temporizador, &QTimer::timeout, this, &MainWindow::actualizarEscena);

    // Dibujar el escenario inicial y actualizar el estado del boton de disparo
    actualizarEstadoTurno();
    dibujarEscenario();
}

MainWindow::~MainWindow()
{
    // Liberar memoria dinamica asignada al simulador y a la interfaz
    delete simulador;
    delete ui;
}

// Convierte coordenada X del mundo fisico (0..100) a pixeles de la escena
double MainWindow::mapX(double xMundo) const
{
    double anchoMundo = simulador->getConfiguracion().getAncho();
    return (xMundo / anchoMundo) * anchoEscena;
}

// Convierte coordenada Y del mundo fisico (0..60, con 0 abajo) a pixeles de Qt (0 arriba)
double MainWindow::mapY(double yMundo) const
{
    double altoMundo = simulador->getConfiguracion().getAlto();
    return altoEscena - ((yMundo / altoMundo) * altoEscena);
}

// Escala un ancho del mundo fisico a pixeles de la escena
double MainWindow::mapAncho(double wMundo) const
{
    double anchoMundo = simulador->getConfiguracion().getAncho();
    return (wMundo / anchoMundo) * anchoEscena;
}

// Escala un alto del mundo fisico a pixeles de la escena
double MainWindow::mapAlto(double hMundo) const
{
    double altoMundo = simulador->getConfiguracion().getAlto();
    return (hMundo / altoMundo) * altoEscena;
}

// Dibuja el canon
void MainWindow::dibujarCanon(double canonX, double canonY, double anguloGrados, bool esJugadorDerecha)
{
    (void)anguloGrados; // Se mantiene el parametro por compatibilidad sin inclinar el sprite

    double px = mapX(canonX);
    double py = mapY(canonY);

    if (!spriteCanon.isNull()) {
        // Escala el sprite directamente conservando su aspecto original
        QPixmap pm = spriteCanon.scaled(
            76, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation
            );

        // Voltea horizontalmente solo para el jugador de la derecha
        if (esJugadorDerecha) {
            pm = pm.transformed(QTransform().scale(-1, 1));
        }

        QGraphicsPixmapItem *itemCanon = new QGraphicsPixmapItem(pm);
        itemCanon->setPos(px - pm.width() / 2.0, py - pm.height() / 2.0);
        escena->addItem(itemCanon);
    } else {
        // Respaldo geometrico si no se carga la imagen
        QGraphicsRectItem *base = new QGraphicsRectItem(px - 24, py - 12, 48, 24);
        base->setBrush(QBrush(Qt::darkGray));
        escena->addItem(base);
    }
}
// Dibuja un muneco de palitos dentro del espacio del jugador si sigue con vida
void MainWindow::dibujarMunecoPalitos(double rx, double ry, double rAncho, double rAlto, bool vivo)
{
    if (!vivo) return;

    double xIzq = mapX(rx);
    double xDer = mapX(rx + rAncho);
    double ySuelo = mapY(ry);
    double yTecho = mapY(ry + rAlto);

    double centroX = (xIzq + xDer) / 2.0;
    double altoTotal = ySuelo - yTecho;

    double radioCabeza = altoTotal * 0.16;
    double yCabezaCentro = yTecho + radioCabeza + 2.0;
    double yCuello = yCabezaCentro + radioCabeza;
    double yCadera = ySuelo - altoTotal * 0.35;
    double yHombros = yCuello + (yCadera - yCuello) * 0.35;
    double anchoBrazos = (xDer - xIzq) * 0.35;
    double anchoPiernas = (xDer - xIzq) * 0.32;

    QPen lapiz(Qt::black, 2);

    // Cabeza
    QGraphicsEllipseItem *cabeza = new QGraphicsEllipseItem(
        centroX - radioCabeza,
        yCabezaCentro - radioCabeza,
        radioCabeza * 2.0,
        radioCabeza * 2.0
        );
    cabeza->setPen(lapiz);
    escena->addItem(cabeza);

    // Cuerpo, brazos y piernas
    escena->addLine(centroX, yCuello, centroX, yCadera, lapiz);
    escena->addLine(centroX - anchoBrazos, yHombros, centroX + anchoBrazos, yHombros, lapiz);
    escena->addLine(centroX, yCadera, centroX - anchoPiernas, ySuelo, lapiz);
    escena->addLine(centroX, yCadera, centroX + anchoPiernas, ySuelo, lapiz);
}

// Redibuja todos los elementos activos de la escena en cada fotograma
void MainWindow::dibujarEscenario()
{
    escena->clear();

    // Dibujar columnas y techos no destruidos junto con su resistencia actual
    const std::vector<Obstaculo*> &obstaculos = simulador->getObstaculos();
    for (size_t i = 0; i < obstaculos.size(); ++i) {
        Obstaculo *obs = obstaculos[i];
        if (obs && !obs->estaDestruido()) {
            double xPx = mapX(obs->get_ejex());
            double yPx = mapY(obs->get_ejey() + obs->get_Alto());
            double wPx = mapAncho(obs->get_Ancho());
            double hPx = mapAlto(obs->get_Alto());

            QGraphicsRectItem *bloque = new QGraphicsRectItem(xPx, yPx, wPx, hPx);
            bloque->setBrush(QBrush(Qt::lightGray));
            bloque->setPen(QPen(Qt::darkGray, 1.5));
            bloque->setZValue(1);
            escena->addItem(bloque);

            // Mostrar la resistencia restante centrada sobre el bloque en cada paso
            int vidaRestante = static_cast<int>(std::ceil(obs->get_resistencia()));
            QGraphicsSimpleTextItem *etiquetaVida = new QGraphicsSimpleTextItem(QString::number(vidaRestante));
            QFont fuente("Arial", 9, QFont::Bold);
            etiquetaVida->setFont(fuente);
            etiquetaVida->setBrush(QBrush(Qt::black));
            etiquetaVida->setZValue(10);

            QRectF rectTexto = etiquetaVida->boundingRect();
            double tx = xPx + (wPx - rectTexto.width()) / 2.0;
            double ty = yPx + (hPx - rectTexto.height()) / 2.0;
            etiquetaVida->setPos(tx, ty);
            escena->addItem(etiquetaVida);
        }
    }
    // Dibujar ambos jugadores (canones enfrentados y munecos de palitos)
    const Jugador *j1 = simulador->getJugador1();
    const Jugador *j2 = simulador->getJugador2();

    if (j1) {
        dibujarCanon(j1->getCanonX(), j1->getCanonY(), j1->getAnguloDisparo(), false);
        dibujarMunecoPalitos(j1->getRivalX(), j1->getRivalY(), j1->getRivalAncho(), j1->getRivalAlto(), j1->estaVivo());
    }
    if (j2) {
        dibujarCanon(j2->getCanonX(), j2->getCanonY(), j2->getAnguloDisparo(), true);
        dibujarMunecoPalitos(j2->getRivalX(), j2->getRivalY(), j2->getRivalAncho(), j2->getRivalAlto(), j2->estaVivo());
    }

    // Dibujar el proyectil activo si se encuentra en movimiento
    const Particula *bala = simulador->getProyectil();
    if (bala && bala->get_activa()) {
        double bx = mapX(bala->get_ejex());
        double by = mapY(bala->get_ejey());
        double rPx = mapAncho(bala->getRadio());
        if (rPx < 4.0) {
            rPx = 4.0;
        }

        QGraphicsEllipseItem *circulo = new QGraphicsEllipseItem(bx - rPx, by - rPx, rPx * 2.0, rPx * 2.0);
        circulo->setBrush(QBrush(Qt::red));
        circulo->setPen(QPen(Qt::darkRed, 1));
        escena->addItem(circulo);
    }
}

// Habilita o bloquea el boton Disparar segun haya un disparo en curso o ganador
void MainWindow::actualizarEstadoTurno()
{
    int ganador = simulador->getGanador();
    bool enCurso = simulador->hayDisparoEnCurso();

    if (ganador != 0) {
        ui->btnDisparar->setEnabled(false);
        return;
    }

    ui->btnDisparar->setEnabled(!enCurso);
}

// Lee el angulo y la velocidad ingresados y solicita el disparo al simulador
void MainWindow::on_btnDisparar_clicked()
{
    if (simulador->hayDisparoEnCurso() || simulador->getGanador() != 0) {
        return;
    }

    bool okAng = false;
    bool okVel = false;
    double angulo = ui->txtAngulo->text().toDouble(&okAng);
    double velocidad = ui->txtVelocidad->text().toDouble(&okVel);

    if (!okAng || !okVel) {
        QMessageBox::warning(this, "Datos invalidos", "Ingresa valores numericos validos para el angulo y la velocidad.");
        return;
    }

    if (simulador->disparar(angulo, velocidad)) {
        actualizarEstadoTurno();
        temporizador->start();
    } else {
        QMessageBox::warning(this, "Parametros fuera de rango", "El angulo debe estar entre 0 y 90 grados.");
    }
}

// Avanza un paso de simulacion, actualiza la pantalla y verifica si hay ganador
void MainWindow::actualizarEscena()
{
    simulador->avanzarPaso();
    dibujarEscenario();
    actualizarEstadoTurno();

    int ganador = simulador->getGanador();
    if (ganador != 0) {
        temporizador->stop();
        QString mensaje = (ganador == 1)
                              ? "El Jugador 1 impacto al rival y gano la partida."
                              : "El Jugador 2 impacto al rival y gano la partida.";
        QMessageBox::information(this, "Partida finalizada", mensaje);
    }
}