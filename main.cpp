#include "latexconvert.h"
#include <QApplication>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  LatexConvert w;
  w.show();

  return a.exec();
}
