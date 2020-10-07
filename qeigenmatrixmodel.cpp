#include "qeigenmatrixmodel.h"
#include <limits>

QEigenMatrixModel::QEigenMatrixModel(Eigen::MatrixXd &matrix)
    : QAbstractTableModel(), m_matrix(matrix) {}

QEigenMatrixModel::~QEigenMatrixModel() {}

int QEigenMatrixModel::rowCount(const QModelIndex &) const {
  return m_matrix.rows();
}

int QEigenMatrixModel::columnCount(const QModelIndex &) const {
  return m_matrix.cols();
}

QVariant QEigenMatrixModel::data(const QModelIndex &index, int role) const {
  switch (role) {
  case Qt::DisplayRole:
    if (index.isValid() && index.column() < m_matrix.cols() &&
        index.row() < m_matrix.rows())
      return QString::number(m_matrix(index.row(), index.column()));
  default:
    break;
  }

  return QVariant();
}

Qt::ItemFlags QEigenMatrixModel::flags(const QModelIndex &index) const {
  return QAbstractItemModel::flags(index);
}

QHash<int, QByteArray> QEigenMatrixModel::roleNames() const {
  return {{Qt::DisplayRole, "display"}};
}
