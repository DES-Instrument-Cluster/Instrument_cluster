#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "speedometer.h"
#include "canreceiver.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateSpeed(double speed);

private:
    Ui::MainWindow	*ui;
    Speedometer		*speedometer;
    CanReceiver		*canReceiver;
};

#endif // MAINWINDOW_H
