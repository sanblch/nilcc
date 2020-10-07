#include "tablemodel.h"
#include <QStringList>
#include <limits>

TableModel::TableModel(Eigen::MatrixXd &matrix)
    : QEigenMatrixModel(matrix) {}

TableModel::~TableModel() {}

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
  emit dataChanged(index(0, 0), index(m_matrix.rows(), m_matrix.cols()));
}
