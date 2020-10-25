#include "mainwindow.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>

MainWindow::MainWindow(QWidget *parent)
    : QQuickWidget(parent), m_path("main.qml") {
  m_watcher.addPath(m_path);
  rootContext()->setContextProperty("tablemodel", m_controller.model());
  setSource(QUrl(m_path));
  connect(&m_watcher, &QFileSystemWatcher::fileChanged,
          [this](const QString &) {
            engine()->clearComponentCache();
            setSource(QUrl(m_path));
            m_watcher.addPath(m_path);
          });
}
