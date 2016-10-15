#include "latexconvert.h"
#include "cpp11-range/range.hpp"
#include "latex/formatfromclipboard.h"
#include "latex/latexcreator.h"
#include "model/configoptions.h"
#include "ui_latexconvert.h"
#include <QClipboard>
#include <QTextEdit>
#include <functional>
#include <utility>

LatexConvert::LatexConvert(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::LatexConvert)
  , itemmodel(new QStandardItemModel(this))
{
  ui->setupUi(this);
  tableview = ui->tableView;
  itemmodel->setColumnCount(5);
  itemmodel->setRowCount(5);
  tableview->setModel(itemmodel);
}
namespace {

QVector<QVector<QString>>
get_model_data(QStandardItemModel* model)
{
  std::pair<int, int> dimensions{ model->rowCount(), model->columnCount() };
  auto data = latex::provision_datastruct(dimensions);
  for (int i{ 0 }; i < dimensions.first; i++) {
    for (int j{ 0 }; j < dimensions.second; j++) {
      auto idx = model->index(i, j);
      QString cell = (model->data(idx)).toString();

      data[i][j] = cell;
    }
  }
  return data;
}
bool
set_model_data(QStandardItemModel* model, QVector<QVector<QString>> data)
{
  if (data.size() == 0)
    return false;
  if (data[0].size() == 0)
    return false;
  model->setRowCount(data.size());
  model->setColumnCount(data[0].size());
  for (int i{ 0 }; i < model->rowCount(); i++) {
    for (int j{ 0 }; j < model->columnCount(); j++) {
      auto idx = model->index(i, j);
      model->setData(idx, data[i][j]);
    }
  }
  return true;
}

QVector<QVector<QString>>
rotate_data(QStandardItemModel* model)
{
  using util::lang::indices;
  std::pair<int, int> dimensions{ model->columnCount(), model->rowCount() };
  auto new_data = latex::provision_datastruct(dimensions);
  auto old_data = get_model_data(model);
  for (auto i : indices(old_data)) {
    for (auto j : indices(old_data[i])) {
      new_data[j][i] = old_data[i][j];
    }
  }
  return new_data;
}
}
LatexConvert::~LatexConvert()
{
  delete ui;
}

void
LatexConvert::on_add_row_button_clicked()
{
  int rows = itemmodel->rowCount();
  itemmodel->setRowCount(++rows);
}

void
LatexConvert::on_add_column_button_clicked()
{
  int cols = itemmodel->columnCount();
  itemmodel->setColumnCount(++cols);
}

void
LatexConvert::on_remove_row_button_clicked()
{
  int rows = itemmodel->rowCount();
  itemmodel->setRowCount(--rows);
}

void
LatexConvert::on_remove_column_button_clicked()
{
  int cols = itemmodel->columnCount();
  itemmodel->setColumnCount(--cols);
}

void
LatexConvert::on_generate_latex_button_clicked()
{

  auto data = get_model_data(itemmodel);
  if (data.size() > 0) {
    auto config = latex::generate_config_options(data.size(), data[0].size(),
                                                 latex::RowType::CENTER_LINES);
    auto latex =
      latex::flatten_latex(latex::create_latex_from_grid_data(data, config));
    ui->textEdit->setPlainText(latex);
    // TODO: Add checkbox for this
    auto* clipboard = QApplication::clipboard();
    clipboard->setText(latex);
  }
}

void
LatexConvert::on_from_clipboard_button_clicked()
{
  const auto* clipboard = QApplication::clipboard();
  auto data = latex::grab_and_format_clipboard(clipboard);
  set_model_data(itemmodel, data);
}

void
LatexConvert::on_swap_row_col_button_clicked()
{
  auto data = rotate_data(itemmodel);
  set_model_data(itemmodel, data);
}
