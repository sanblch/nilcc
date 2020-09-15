#include "controller.h"
#include "conc.h"
#include "tablemodel.h"
#include <iostream>

Controller::Controller() {
  m_model = new TableModel(m_matrix);
  connect(m_model, &QAbstractItemModel::dataChanged,
          [this](const QModelIndex &, const QModelIndex &) {
            auto basis = m_matrix.cols() - 2;
            std::cout << m_matrix.cols() << std::endl;
            MatrixXd B;
            B.resize(1, basis);
            B << 0.00481452, 0.01000039;
            double h = 1.0;
            Sysc sysc;
            sysc.concAlg = ConcAlg::BUGAEVSKY;
            sysc.verb = true;
            while (h < 14) {
              std::cout << "************************* " << h
                        << " *******************************" << std::endl;
              m_result[{h}] =
                  nfconc(m_matrix.leftCols(basis + 1), m_matrix.rightCols(1), B,
                         std::exp(std::pow(10.0, -h)), sysc);
              h += 0.5;
            }
          });
}

Controller::~Controller() {
    delete m_model; }

TableModel *Controller::model() {
    return m_model; }

void Controller::paste() {
  qDebug() << "Pasting";
  const auto *mimeData = QApplication::clipboard()->mimeData();
  if(mimeData->hasText()) {
    qDebug() << mimeData->text();
    m_model->setText(mimeData->text());
  }
}
