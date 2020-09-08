#pragma once

#include "tablemodel.h"
#include <QApplication>
#include <QClipboard>
#include <QMimeData>

class Controller : public QObject {
  Q_OBJECT
  TableModel *m_model;

public:
  explicit Controller();
  virtual ~Controller() override;
  TableModel *model();

public slots:
  void paste();
};
