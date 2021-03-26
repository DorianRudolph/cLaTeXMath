#if (defined(BUILD_QT) or defined(BUILD_SKIA)) && !defined(MEM_CHECK)

#include "latex.h"
#include "samples.h"

#include "qt_mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char **argv)
{
  QApplication::setStyle(QStyleFactory::create("Fusion"));
  QApplication app(argc, argv);

  tex::LaTeX::init();
  MainWindow mainwin;
  mainwin.show();
  int retn = app.exec();

  tex::LaTeX::release();
  return retn;
}

#endif
