#pragma once

#include "controller.h"
#include <QFileSystemWatcher>
#include <QQuickWidget>

class MainWindow : public QQuickWidget {
  Q_OBJECT
  Controller m_controller;
  QFileSystemWatcher m_watcher;
  QString m_path;

public:
  explicit MainWindow(QWidget* parent = nullptr);
};
