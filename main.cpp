#include "controller.h"
#include "tablemodel.h"
#include <QApplication>
#include <QClipboard>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    Controller controller;
    engine.rootContext()->setContextProperty("tablemodel", controller.model());
    engine.load(url);

    QObject::connect(engine.rootObjects().first(), SIGNAL(paste()), &controller,
                     SLOT(paste()));
    for(auto obj : engine.rootObjects().first()->children()) {
        if(obj->objectName() == "chart")
            controller.setChart(obj);
    }

    return app.exec();
}
