#include <QThread>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , canReceiverThread(new CanReceiverThread(this))
{
    this->ui->setupUi(this);
    this->setWindowTitle("Instrument Cluster");
    this->setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    this->setStyleSheet("background-color: black");

    this->speedometer = new Speedometer(this);
    setCentralWidget(this->speedometer);

    this->filterOnOffButton = new QPushButton(this);
    this->filterOnOffButton->setGeometry(QRect(QPoint(25, 25), QSize(120, 40)));
    this->setFilterButtonOffStyle();

    connect(this->filterOnOffButton
            , &QPushButton::clicked
            , this
            , &MainWindow::onFilterOnOffButtonClicked);


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
    delete this->ui;
}

void MainWindow::updateSpeed(double speed)
{
    this->speedometer->setSpeed(speed);
}

void MainWindow::onFilterOnOffButtonClicked()
{
    Filter& filter = this->canReceiverThread->getCanReceiver()->getFilter();

    if (filter.getIsFilterOn()) {
        this->setFilterButtonOffStyle();
        filter.setIsFilterOn(false);
    } else {
        this->setFilterButtonOnStyle();
        filter.setIsFilterOn(true);
    }
}

void MainWindow::setFilterButtonOnStyle()
{
    this->filterOnOffButton->setText("Filter ON");
    this->filterOnOffButton->setStyleSheet("background-color: white; color: black; border-radius: 5px;");
}

void MainWindow::setFilterButtonOffStyle()
{
    this->filterOnOffButton->setText("Filter OFF");
    this->filterOnOffButton->setStyleSheet("background-color: grey; color: white; border-radius: 5px;");
}
