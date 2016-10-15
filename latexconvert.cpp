#include "latexconvert.h"
#include "latex/latexcreator.h"
#include "model/configoptions.h"
#include "ui_latexconvert.h"
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
provision_datastruct(std::pair<int, int> dimensions)
{
  QVector<QVector<QString>> res(dimensions.first);
  int columns{ dimensions.second };
  std::for_each(res.begin(), res.end(),
                [columns](auto& vec) { vec = QVector<QString>(columns); });
  return res;
}

QVector<QVector<QString>>
get_model_data(QStandardItemModel* model)
{
  std::pair<int, int> dimensions{ model->rowCount(), model->columnCount() };
  auto data = provision_datastruct(dimensions);
  for (int i{ 0 }; i < dimensions.first; i++) {
    for (int j{ 0 }; j < dimensions.second; j++) {
      auto idx = model->index(i, j);
      QString cell = (model->data(idx)).toString();

      data[i][j] = cell;
    }
  }
  return data;
};
}
LatexConvert::~LatexConvert()
{
  delete ui;
}

void
LatexConvert::on_pushButton_clicked()
{
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
  auto config = latex::generate_config_options(data.size(), data[0].size(),
                                               latex::RowType::CENTER_LINES);
  auto latex = latex::create_latex_from_grid_data(data, config);
  ui->textEdit->setPlainText(latex::flatten_latex(latex));
}
