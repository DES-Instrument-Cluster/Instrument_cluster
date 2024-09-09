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
        SpeedUpdateManager speedUpdateManager;

        engine.rootContext()->setContextProperty("batteryChecker", &batteryChecker);
        engine.rootContext()->setContextProperty("speedUpdateManager", &speedUpdateManager);
        engine.rootContext()->setContextProperty("filterManager", speedUpdateManager.getFilterManager());

        batteryChecker.startMonitoring();
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
