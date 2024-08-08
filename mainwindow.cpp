#include <QThread>

#include "mainwindow.h"
#include "speedometer.h"
#include "canreceiver.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canReceiver(new CanReceiver(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Instrument Cluster");
    this->setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    this->setStyleSheet("background-color: black");

    this->speedometer = new Speedometer(this);
    setCentralWidget(this->speedometer);

    QThread *canThread = new QThread;
    this->canReceiver->moveToThread(canThread);
    connect(canThread, &QThread::started, this->canReceiver, [this]() {
        canReceiver->startReceiving("can1");
    });

    connect(this->canReceiver, &CanReceiver::speedUpdated, this, &MainWindow::updateSpeed);
    connect(canThread, &QThread::finished, this->canReceiver, &CanReceiver::deleteLater);
    connect(canThread, &QThread::finished, canThread, &QThread::deleteLater);

    canThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSpeed(double speed)
{
    this->speedometer->setSpeed(speed);
}
