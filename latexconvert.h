#ifndef LATEXCONVERT_H
#define LATEXCONVERT_H

#include <QMainWindow>

namespace Ui {
class LatexConvert;
}

class LatexConvert : public QMainWindow
{
    Q_OBJECT

public:
    explicit LatexConvert(QWidget *parent = 0);
    ~LatexConvert();

private:
    Ui::LatexConvert *ui;
};

#endif // LATEXCONVERT_H
