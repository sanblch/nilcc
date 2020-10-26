#pragma once

#include "controller.h"
#include <QQmlApplicationEngine>

class MainWindow : public QObject {
  Q_OBJECT
  Controller m_controller;
  QQmlApplicationEngine engine;
  QString m_path;

public:
  explicit MainWindow(QObject *parent = nullptr);
};
