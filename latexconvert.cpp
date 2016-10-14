#include "latexconvert.h"
#include "ui_latexconvert.h"

LatexConvert::LatexConvert(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LatexConvert)
{
    ui->setupUi(this);
}

LatexConvert::~LatexConvert()
{
    delete ui;
}
