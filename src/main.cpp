#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "canreceiver.h"
#include "batterychecker.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    BatteryChecker batteryChecker;
    engine.rootContext()->setContextProperty("batteryChecker", &batteryChecker);
    qmlRegisterType<BatteryChecker>("InstrumentCluster", 1, 0, "BatteryChecker");
    batteryChecker.monitor();

    CanReceiver canReceiver;
    engine.rootContext()->setContextProperty("canReceiver", &canReceiver);
    qmlRegisterType<CanReceiver>("InstrumentCluster", 1, 0, "CanReceiver");

    canReceiver.startReceiving("can1");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
