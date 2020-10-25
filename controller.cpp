#include "controller.h"
#include "conc.h"
#include "tablemodel.h"
#include <QLineSeries>
#include <QValueAxis>
#include <iostream>

Controller::Controller() {
  m_model = new TableModel(m_matrix);
  connect(m_model, &QAbstractItemModel::dataChanged, [this](const QModelIndex &, const QModelIndex &) {
      auto basis = m_matrix.cols() - 2;
      std::cout << m_matrix.cols() << std::endl;
      MatrixXd B;
      B.resize(1, basis);
      B << 0.00481452, 0.01000039;
      Sysc sysc;
      sysc.concAlg = ConcAlg::BUGAEVSKY;
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
          std::cout << "************************* " << m_x[i] << " *******************************"
                    << std::endl;
          VectorXd h(1);
          h.coeffRef(0) = std::log(std::pow(10.0, -m_x[i]));
          result.row(i) = nfconc(m_matrix.leftCols(basis + 1), m_matrix.rightCols(1), B, h, sysc);
          std::cout << result.row(i) << std::endl;
      }
      emit calculated();
  });
}

Controller::~Controller() {
    delete m_model; }

void Controller::setChart(QObject *chart) {
  m_chart = chart;
}

TableModel *Controller::model() {
    return m_model;
}

std::vector<int> Controller::dim()
{
    return m_dim;
}

std::vector<double> Controller::result()
{
    return m_result;
}

std::vector<double> Controller::x()
{
    return m_x;
}

void Controller::paste()
{
    qDebug() << "Pasting";
    //  const auto *mimeData = QApplication::clipboard()->mimeData();
    //  if(mimeData->hasText()) {
    //    qDebug() << mimeData->text();
    //    m_model->setText(mimeData->text());
    //  } else {
    m_model->setText("1	0	0	0\n"
                     "0	1	0	0\n"
                     "0	1	-1	-5,5729\n"
                     "0	1	-2	-18,9986\n"
                     "0	1	-3	-40,1893\n"
                     "1	1	-2	-6,9054\n"
                     "1	1	-3	-17,0345\n"
                     "1	2	-4	-16,1181\n"
                     "1	2	-5	-24,937\n"
                     "1	2	-6	-38,9666\n"
                     "1	2	-7	-65,7388\n");
    //  }
}
