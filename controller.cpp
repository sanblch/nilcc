#include "controller.h"
#include "conc.h"
#include "tablemodel.h"
#include <QLineSeries>
#include <QValueAxis>
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
            int count = 0;
            while (h < 14) {
              h += 0.5;
              count++;
            }
            m_result.resize(count, m_matrix.rows() + 1);
            count = 0;
            h = 1.0;
            while (h < 14) {
              std::cout << "************************* " << h
                        << " *******************************" << std::endl;
              m_result(count, 0) = h;
              m_result.row(count++).tail(m_matrix.rows()) =
                  nfconc(m_matrix.leftCols(basis + 1), m_matrix.rightCols(1), B,
                         std::exp(std::pow(10.0, -h)), sysc);
              h += 0.5;
            }
            if (m_chart) {
              QtCharts::QLineSeries *series;
              QtCharts::QValueAxis axisX;
              axisX.setRange(0, 15);
              QtCharts::QValueAxis axisY;
              axisY.setRange(0, 1);
              try {
                QMetaObject::invokeMethod(
                    m_chart, "createSeries",
                    Q_RETURN_ARG(QtCharts::QLineSeries *, series),
                    Q_ARG(int, static_cast<int>(
                                   QtCharts::QAbstractSeries::SeriesTypeLine)),
                    Q_ARG(QString, "series"),
                    Q_ARG(QtCharts::QAbstractAxis *, &axisX),
                    Q_ARG(QtCharts::QAbstractAxis *, &axisY));
              }
              catch(...) {
                  qDebug() << "ZVIZDETS!";
              }

              if (series) {
                qDebug() << "CREATED!";
                auto serie = new QtCharts::QLineSeries;
                serie->append(1, 5);
                serie->append(2, 10);
              }
            }
          });
}

Controller::~Controller() {
    delete m_model; }

void Controller::setChart(QObject *chart) {
  m_chart = chart;
}

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
