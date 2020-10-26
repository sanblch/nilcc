#include "mainwindow.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>

MainWindow::MainWindow(QObject *parent) : m_path("qrc:/main.qml") {
  engine.rootContext()->setContextProperty("tablemodel", m_controller.model());
  engine.rootContext()->setContextProperty("backend", &m_controller);
  engine.load(QUrl(m_path));
  connect(engine.rootObjects().first(), SIGNAL(paste()), &m_controller,
          SLOT(paste()));
  connect(&m_controller, SIGNAL(calculated()), engine.rootObjects().first(),
          SLOT(refresh()));
}
