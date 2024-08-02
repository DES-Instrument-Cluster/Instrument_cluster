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
    this->setFixedSize(1280, 400);
    this->setStyleSheet("background-color: black");

    this->speedometer = new Speedometer(this);
    setCentralWidget(this->speedometer);

    connect(canReceiver, &CanReceiver::speedUpdated, this, &MainWindow::updateSpeed);

    canReceiver->startReceiving("can1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSpeed(double speed)
{
    this->speedometer->setSpeed(speed);
}
