#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "qeigenmatrixmodel.h"
#include <qqml.h>

class TableModel : public QEigenMatrixModel {
  Q_OBJECT
  QML_ELEMENT
  QML_ADDED_IN_MINOR_VERSION(1)

  double parseDouble(const QString &data);

  public:
  explicit TableModel(Eigen::MatrixXd& matrix);
  virtual ~TableModel() override;
  void setText(const QString &str);
  Q_INVOKABLE void setData(int row, int column, const QVariant &data);
};

#endif // TABLEMODEL_H
