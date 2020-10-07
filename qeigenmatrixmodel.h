#ifndef QEIGENMATRIXMODEL_H
#define QEIGENMATRIXMODEL_H

#include <Eigen/Dense>
#include <QAbstractTableModel>
#include <QDebug>
#include <qqml.h>

class QEigenMatrixModel : public QAbstractTableModel {
  Q_OBJECT
  QML_ELEMENT
  QML_ADDED_IN_MINOR_VERSION(1)

public:
  explicit QEigenMatrixModel(Eigen::MatrixXd &matrix);
  virtual ~QEigenMatrixModel() override;
  int rowCount(const QModelIndex & = QModelIndex()) const override;
  int columnCount(const QModelIndex & = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QHash<int, QByteArray> roleNames() const override;

protected:
  Eigen::MatrixXd &m_matrix;
};

#endif // QEIGENMATRIXMODEL_H
