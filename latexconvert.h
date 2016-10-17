#ifndef LATEXCONVERT_H
#define LATEXCONVERT_H

#include "GSL/gsl/gsl"
#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>

using gsl::not_null;

namespace Ui {
class LatexConvert;
}

class LatexConvert : public QMainWindow
{
  Q_OBJECT

public:
  explicit LatexConvert(QWidget* parent = 0);
  ~LatexConvert();

private slots:
  void on_add_column_button_clicked();

  void on_add_row_button_clicked();

  void on_remove_row_button_clicked();

  void on_remove_column_button_clicked();

  void on_generate_latex_button_clicked();

  void on_from_clipboard_button_clicked();

  void on_swap_row_col_button_clicked();

private:
  void add_new_data_model(std::pair<int, int> rowcol,
                          QVector<QVector<QString>>&& grid_data);
  Ui::LatexConvert* ui;
  not_null<QStandardItemModel*> itemmodel;
};

#endif // LATEXCONVERT_H
