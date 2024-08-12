#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "speedometer.h"
#include "canreceiverthread.h"
#include <QPushButton>
#include <QWidget>
#include "emafilter.h"

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
    void onFlatterFilterButtonClicked();

private:
    Ui::MainWindow		*ui;
    Speedometer			*speedometer;
    CanReceiverThread	*canReceiverThread;
    QPushButton *flatterFilterButton;
    bool isBlack;

    const int	SCREEN_WIDTH = 1280;
    const int	SCREEN_HEIGHT = 400;

};

#endif // MAINWINDOW_H
