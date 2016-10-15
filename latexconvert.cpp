#include "latexconvert.h"
#include "latex/latexcreator.h"
#include "model/configoptions.h"
#include "ui_latexconvert.h"
#include <QTextEdit>
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
  if (!tableptr)
    return;
  auto grid_data = tableptr->get_grid_data();
  grid_data = tablemodel::addRow(grid_data);

  std::pair<int, int> rowcol = { tableptr->rowCount() + 1,
                                 tableptr->columnCount() };
  add_new_data_model(rowcol, std::move(grid_data));
}

void
LatexConvert::on_add_column_button_clicked()
{
  if (!tableptr)
    return;
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
  tableptr.reset(nullptr);
  tableptr = std::make_unique<TableModel>(nullptr, rowcol.first, rowcol.second,
                                          std::move(grid_data));
  tableview->setModel(tableptr.get());
}

void
LatexConvert::on_remove_row_button_clicked()
{
  if (!tableptr)
    return;
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
  if (!tableptr)
    return;
  if (tableptr->columnCount() > 1) {
    auto grid_data = tableptr->get_grid_data();
    grid_data = tablemodel::removeColumn(grid_data);

    std::pair<int, int> rowcol = { tableptr->rowCount(),
                                   tableptr->columnCount() - 1 };
    add_new_data_model(rowcol, std::move(grid_data));
  }
}

void
LatexConvert::on_generate_latex_button_clicked()
{
  if (!tableptr)
    return;
  auto& table = *(tableptr.get());
  auto configopts = latex::generate_config_options(
    table.rowCount(), table.columnCount(), latex::RowType::CENTER_LINES);
  auto latex =
    latex::create_latex_from_grid_data(table.get_c_grid_data(), configopts);
  auto unwrap_text = [](const auto& textlines) {
    QString text;
    std::for_each(
      textlines.cbegin(), textlines.cend(),
      [&text](const auto& line) { text.append(QString("%1\n").arg(line)); });
    return text;
  };
  ui->textEdit->setPlainText(unwrap_text(latex));
}
