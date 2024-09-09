#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "speedupdatemanager.h"
#include "batterychecker.h"

int main(int argc, char *argv[])
{
    try
    {
        QGuiApplication app(argc, argv);
        QQuickStyle::setStyle("Material");
        QQmlApplicationEngine engine;
        BatteryChecker batteryChecker;
        engine.rootContext()->setContextProperty("batteryChecker", &batteryChecker);
        qmlRegisterType<BatteryChecker>("InstrumentCluster", 1, 0, "BatteryChecker");
        batteryChecker.startMonitoring();

        SpeedUpdateManager speedUpdateManager;

        engine.rootContext()->setContextProperty("speedUpdateManager", &speedUpdateManager);
        qmlRegisterType<SpeedUpdateManager>("InstrumentCluster", 1, 0, "SpeedUpdateManager");

        engine.rootContext()->setContextProperty("filterManager", speedUpdateManager.getFilterManager());
        qmlRegisterType<FilterManager>("InstrumentCluster", 1, 0, "SpeedUpdateManager");

        speedUpdateManager.start();

        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty())
        {
            return 1;
        }

        return app.exec();
    }
    catch (std::exception& e)
    {
        qDebug() << e.what() << '\n';
        return 1;
    }
}
