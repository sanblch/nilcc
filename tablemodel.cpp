#include "tablemodel.h"
#include <limits>

TableModel::TableModel() : QAbstractTableModel() {}
TableModel::~TableModel() {}

int TableModel::rowCount(const QModelIndex &) const {
  return m_matrix.rows();
}

int TableModel::columnCount(const QModelIndex &) const {
  return m_matrix.cols();
}

void TableModel::setText(const QString& str) {
  qDebug() << str;
  auto lst = str.split("\n", QString::SkipEmptyParts);
  size_t size = 0;
  QList<QStringList> llst;
  for (const auto &i : lst) {
    llst.append(i.split("\t"));
    if (llst.last().size() > size)
      size = llst.last().size();
  }
  beginResetModel();
  m_matrix.resize(llst.size(), size);
  size_t ci = 0;
  size_t cj = 0;
  for (const auto &i : llst) {
    cj = 0;
    for (const auto &j : i) {
      bool ok = false;
      double val = j.toDouble(&ok);
      m_matrix(ci, cj) = ok ? val : std::numeric_limits<double>::quiet_NaN();
      ++cj;
    }
    ++ci;
  }
  endResetModel();
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
  switch (role) {
  case Qt::DisplayRole:
    if(index.isValid() && index.column() < m_matrix.cols() && index.row() < m_matrix.rows())
      return QString::number(m_matrix(index.row(), index.column()));
  default:
    break;
  }

  return QVariant();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const {
  return QAbstractItemModel::flags(index);
}

QHash<int, QByteArray> TableModel::roleNames() const {
  return {{Qt::DisplayRole, "display"}};
}
