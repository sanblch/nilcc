#pragma once

#include "tablemodel.h"
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <map>

class Controller : public QObject {
  Q_OBJECT
  TableModel *m_model;
  Eigen::MatrixXd m_matrix;
  std::map<std::vector<double>, Eigen::MatrixXd> m_result;

public:
  explicit Controller();
  virtual ~Controller() override;
  TableModel *model();

public slots:
  void paste();
};
