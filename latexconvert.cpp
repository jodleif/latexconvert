#include "latexconvert.h"
#include "ui_latexconvert.h"
#include <utility>

LatexConvert::LatexConvert(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::LatexConvert)
{
  ui->setupUi(this);
  tableview = ui->tableView;
}

LatexConvert::~LatexConvert()
{
  delete ui;
}

void
LatexConvert::on_pushButton_clicked()
{
  tableview->setModel(nullptr);
  tableptr = std::make_unique<TableModel>(nullptr, 2, 2);
  tableview->setModel(tableptr.get());
}

void
LatexConvert::on_add_row_button_clicked()
{
  auto grid_data = tableptr->get_grid_data();
  grid_data = tablemodel::addRow(grid_data);

  std::pair<int, int> rowcol = { tableptr->rowCount() + 1,
                                 tableptr->columnCount() };
  add_new_data_model(rowcol, std::move(grid_data));
}

void
LatexConvert::on_add_column_button_clicked()
{
  auto grid_data = tableptr->get_grid_data();
  grid_data = tablemodel::addColumn(grid_data);
  std::pair<int, int> rowcol = { tableptr->rowCount(),
                                 tableptr->columnCount() + 1 };
  add_new_data_model(rowcol, std::move(grid_data));
}

void
LatexConvert::add_new_data_model(std::pair<int, int> rowcol,
                                 QVector<QVector<QString>>&& grid_data)
{
  tableview->setModel(nullptr);
  tableview->update();
  tableptr = std::make_unique<TableModel>(nullptr, rowcol.first, rowcol.second,
                                          std::move(grid_data));
  tableview->setModel(tableptr.get());
}

void
LatexConvert::on_remove_row_button_clicked()
{
  if (tableptr->rowCount() > 1) {
    auto grid_data = tableptr->get_grid_data();
    grid_data = tablemodel::removeRow(grid_data);

    std::pair<int, int> rowcol = { tableptr->rowCount() - 1,
                                   tableptr->columnCount() };
    add_new_data_model(rowcol, std::move(grid_data));
  }
}

void
LatexConvert::on_remove_column_button_clicked()
{
  if (tableptr->columnCount() > 1) {
    auto grid_data = tableptr->get_grid_data();
    grid_data = tablemodel::removeColumn(grid_data);

    std::pair<int, int> rowcol = { tableptr->rowCount(),
                                   tableptr->columnCount() - 1 };
    add_new_data_model(rowcol, std::move(grid_data));
  }
}
