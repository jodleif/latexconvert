#include "latexconvert.h"
#include "cpp11-range/range.hpp"
#include "latex/formatfromclipboard.h"
#include "latex/latexcreator.h"
#include "model/configoptions.h"
#include "ui_latexconvert.h"
#include <QCheckBox>
#include <QClipboard>
#include <QMessageBox>
#include <QMimeData>
#include <QTextEdit>
#include <experimental/optional>
#include <functional>
#include <utility>

using util::lang::indices;
using util::lang::range;

LatexConvert::LatexConvert(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::LatexConvert)
  , itemmodel(new QStandardItemModel(this))
{
  ui->setupUi(this);
  itemmodel->setColumnCount(5);
  itemmodel->setRowCount(5);
  ui->tableView->setModel(itemmodel);
}
namespace {

QVector<QVector<QString>>
get_model_data(QStandardItemModel* model)
{
  std::pair<int, int> dimensions{ model->rowCount(), model->columnCount() };
  auto data = latex::provision_datastruct(dimensions);

  for (int i : range(0, dimensions.first)) {
    for (int j : range(0, dimensions.second)) {
      auto idx = model->index(i, j);
      QString cell = (model->data(idx)).toString();

      data[i][j] = std::move(cell);
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

  for (auto i : indices(data)) {
    for (int j : indices(data[0])) {
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

void
show_error_dialog(const QString message, const QString detailed_error = "")
{
  QMessageBox error_dialog;
  error_dialog.setText(message);
  error_dialog.setIcon(QMessageBox::Warning);
  if (detailed_error.size() != 0) {
    error_dialog.setDetailedText(detailed_error);
  }
  error_dialog.exec();
}

const QMimeData*
get_clipboard_contents()
{
  const auto* clipboard = QApplication::clipboard();
  return clipboard->mimeData();
}

void
handle_clipboard_error(not_null<const QMimeData*> clipboard)
{
  if (clipboard->hasText()) {
    show_error_dialog(
      "Could not load clipboard data. Clipboard contents below (show details)",
      clipboard->text());
  } else {
    show_error_dialog("Clipboard has no text!");
  }
}
} // anon namespace end
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

    auto copy_to_clipboard = ui->clipboardCopy->isChecked();
    if (copy_to_clipboard) {
      auto* clipboard = QApplication::clipboard();
      clipboard->setText(latex);
    }
  }
}

void
LatexConvert::on_from_clipboard_button_clicked()
{
  auto clipboard = get_clipboard_contents(); // QMimeData

  if (clipboard != nullptr) {
    auto non_null_clipboard = not_null<const QMimeData*>(clipboard);
    auto data = // std::optional
      latex::grab_and_format_clipboard(non_null_clipboard);
    if (data)
      set_model_data(itemmodel, *data);
    else {
      handle_clipboard_error(non_null_clipboard);
    }
  }
}

void
LatexConvert::on_swap_row_col_button_clicked()
{
  auto data = rotate_data(itemmodel);
  set_model_data(itemmodel, data);
}
