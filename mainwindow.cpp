#include "mainwindow.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>

MainWindow::MainWindow(QWidget *parent)
    : QQuickWidget(parent), m_path("main.qml") {
  m_watcher.addPath(m_path);
  rootContext()->setContextProperty("tablemodel", m_controller.model());
  rootContext()->setContextProperty("backend", &m_controller);
  setSource(QUrl(m_path));
  setResizeMode(QQuickWidget::SizeRootObjectToView);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(&m_watcher, &QFileSystemWatcher::fileChanged, [this](const QString &) {
      engine()->clearComponentCache();
      setSource(QUrl(m_path));
      m_watcher.addPath(m_path);
  });
  connect(rootObject(), SIGNAL(paste()), &m_controller, SLOT(paste()));
  connect(&m_controller, SIGNAL(calculated()), rootObject(), SLOT(refresh()));
}
