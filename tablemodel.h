#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <qqml.h>
#include <QDebug>
#include <Eigen/Dense>

class TableModel : public QAbstractTableModel {
  Q_OBJECT
  QML_ELEMENT
  QML_ADDED_IN_MINOR_VERSION(1)
  Eigen::MatrixXd m_matrix;

public:
  explicit TableModel();
  virtual ~TableModel() override;
  int rowCount(const QModelIndex & = QModelIndex()) const override;
  int columnCount(const QModelIndex & = QModelIndex()) const override;
  void setText(const QString &str);
  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QHash<int, QByteArray> roleNames() const override;
};

#endif // TABLEMODEL_H
