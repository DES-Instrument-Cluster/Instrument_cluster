#include <QThread>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canReceiverThread(new CanReceiverThread(this))
{
    isBlack = false;
    ui->setupUi(this);
    this->setWindowTitle("Instrument Cluster");
    this->setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    this->setStyleSheet("background-color: black");

    this->speedometer = new Speedometer(this);
    setCentralWidget(this->speedometer);
    flatterFilterButton = new QPushButton("FlatterFilter", this);
    flatterFilterButton->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));

    // Устанавливаем начальный стиль кнопки: серый фон и белый текст
    flatterFilterButton->setStyleSheet("background-color: black; color: white;  border: 1px solid white;");

    // Подключаем сигнал нажатия кнопки к слоту
    connect(flatterFilterButton
            , &QPushButton::clicked
            , this, &MainWindow::onFlatterFilterButtonClicked);


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

void MainWindow::onFlatterFilterButtonClicked() {
    if (isBlack) {
        flatterFilterButton->setStyleSheet("background-color: black; color: white; border: 1px solid white;");  // Изменяем цвет на серый с белой рамкой
         // Включаем фильтр с параметром 1
        canReceiverThread->getCanReceiver()->getEmaFilter().setAlpha(1.0);

    } else {
        flatterFilterButton->setStyleSheet("background-color: green; color: white; border: 2px solid white;"); // Изменяем цвет на зеленый с белой рамкой
        // Включаем фильтр без дополнительных параметров
        canReceiverThread->getCanReceiver()->getEmaFilter().setAlpha(0.25);
    }
    isBlack = !isBlack;  // Переключаем состояние переменной
}
