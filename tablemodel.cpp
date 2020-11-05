#include "tablemodel.h"
#include <QStringList>
#include <limits>

TableModel::TableModel(Eigen::MatrixXd &matrix)
    : QEigenMatrixModel(matrix) {}

TableModel::~TableModel() {}

double TableModel::parseDouble(const QString &data)
{
    bool ok = false;
    QLocale russian(QLocale::Russian);
    double val = data.toDouble(&ok);
    if (!ok)
        val = russian.toDouble(data, &ok);
    return ok ? val : std::numeric_limits<double>::quiet_NaN();
}

void TableModel::setText(const QString &str)
{
    qDebug() << str;
    auto lst = str.split("\n", QString::SkipEmptyParts);
    size_t size = 0;
    QList<QStringList> llst;
    for (const auto &i : lst) {
      llst.append(i.split("\t", QString::SkipEmptyParts));
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
            m_matrix(ci, cj) = parseDouble(j);
            ++cj;
        }
        ++ci;
    }
    endResetModel();
    emit dataChanged(index(0, 0), index(m_matrix.rows(), m_matrix.cols()));
}

void TableModel::setData(int row, int column, const QVariant &data)
{
    beginResetModel();
    m_matrix(row, column) = parseDouble(data.toString());
    endResetModel();
    emit dataChanged(index(row, column), index(row, column));
}
