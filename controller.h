#pragma once

#include "tablemodel.h"
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <map>

class QLineSeries;

class Controller : public QObject {
  Q_OBJECT
  QObject *m_chart;
  TableModel *m_model;
  Eigen::MatrixXd m_matrix;
  Eigen::MatrixXd m_result;

public:
  explicit Controller();
  virtual ~Controller() override;

  void setChart(QObject* chart);
  TableModel *model();

public slots:
  void paste();
};
