#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  //    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QApplication app(argc, argv);

  MainWindow window;

  //  QObject::connect(engine.rootObjects().first(), SIGNAL(paste()),
  //  &controller,
  //                   SLOT(paste()));
  //  for (auto obj : engine.rootObjects().first()->children()) {
  //    if (obj->objectName() == "chart")
  //      controller.setChart(obj);
  //  }

  window.show();

  return app.exec();
}
