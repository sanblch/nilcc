#include "controller.h"
#include "conc.h"
#include "tablemodel.h"
#include <QLineSeries>
#include <QValueAxis>
#include <iostream>

Controller::Controller() {
  m_init = new TableModel(m_conc);
  connect(m_init, &QAbstractItemModel::dataChanged,
          [this](const QModelIndex &, const QModelIndex &) { calculate(); });
  m_model = new TableModel(m_matrix);
  connect(m_model, &QAbstractItemModel::dataChanged,
          [this](const QModelIndex &, const QModelIndex &) { calculate(); });
}

Controller::~Controller() {
  delete m_init;
  delete m_model;
}

void Controller::calculate() {
  if (!m_conc.size() || !m_matrix.size())
    return;
  auto basis = m_matrix.cols() - 2;
  std::cout << m_matrix.cols() << std::endl;
  MatrixXd B;
  B.resize(1, basis);
  B.row(0) = m_conc.row(0);
  Sysc sysc;
  sysc.concAlg = ConcAlg::BRINKLEY;
  sysc.verb = true;
  m_x.clear();
  m_x.push_back(1.0);
  while (m_x[m_x.size() - 1] < 14)
    m_x.push_back(m_x[m_x.size() - 1] + 0.5);
  m_dim = {static_cast<int>(m_x.size()), static_cast<int>(m_matrix.rows())};
  m_result.resize(m_dim[0] * m_dim[1]);
  Map<MatrixXd> result(m_result.data(), m_dim[0], m_dim[1]);
  Map<VectorXd> h(m_x.data(), m_x.size());
  for (unsigned i = 0; i < m_x.size(); ++i) {
    std::cout << "************************* " << m_x[i]
              << " *******************************" << std::endl;
    VectorXd h(1);
    h.coeffRef(0) = std::log(std::pow(10.0, -m_x[i]));
    result.row(i) = nfconc(m_matrix.leftCols(basis + 1), m_matrix.rightCols(1),
                           B, h, m_conc.row(1), sysc);
    std::cout << result.row(i) << std::endl;
  }
  emit calculated();
}

void Controller::setChart(QObject *chart) { m_chart = chart; }

TableModel *Controller::init() { return m_init; }

TableModel *Controller::model() { return m_model; }

std::vector<int> Controller::dim() { return m_dim; }

std::vector<double> Controller::result() { return m_result; }

std::vector<double> Controller::x() { return m_x; }

void Controller::paste() {
  qDebug() << "Pasting";
  const auto *mimeData = QApplication::clipboard()->mimeData();
  if (mimeData->hasText()) {
    auto str = mimeData->text();
    auto index = str.indexOf('\n', str.indexOf('\n') + 1) + 1;
    m_init->setText(str.left(index));
    m_model->setText(str.mid(index));
  }
}
