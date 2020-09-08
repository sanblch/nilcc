#include "controller.h"
#include "tablemodel.h"

Controller::Controller() { m_model = new TableModel; }

Controller::~Controller() { delete m_model; }

TableModel *Controller::model() { return m_model; }

void Controller::paste() {
  const auto *mimeData = QApplication::clipboard()->mimeData();
  if(mimeData->hasText())
    m_model->setText(mimeData->text());
}
