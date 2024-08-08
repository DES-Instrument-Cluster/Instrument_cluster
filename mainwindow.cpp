#include <QThread>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canReceiverThread(new CanReceiverThread(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Instrument Cluster");
    this->setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    this->setStyleSheet("background-color: black");

    this->speedometer = new Speedometer(this);
    setCentralWidget(this->speedometer);

    connect(this->canReceiverThread->getCanReceiver()
            , &CanReceiver::speedUpdated
            , this
            , &MainWindow::updateSpeed);
    connect(this->canReceiverThread
            , &QThread::finished
            , this->canReceiverThread->getCanReceiver()
            , &CanReceiver::deleteLater);
    connect(this->canReceiverThread
            , &QThread::finished
            , this->canReceiverThread->getCanReceiver()
            , &QThread::deleteLater);

    this->canReceiverThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSpeed(double speed)
{
    this->speedometer->setSpeed(speed);
}
