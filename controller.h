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
  std::vector<int> m_dim;
  std::vector<double> m_result;
  std::vector<double> m_x;

  public:
  explicit Controller();
  virtual ~Controller() override;

  void setChart(QObject* chart);
  TableModel *model();

  Q_INVOKABLE std::vector<int> dim();
  Q_INVOKABLE std::vector<double> x();
  Q_INVOKABLE std::vector<double> result();

  signals:
  void calculated();

  public slots:
  void paste();
};
