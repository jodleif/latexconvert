#include "latexcreator.h"
#include <iostream>
#include <string>

namespace {
constexpr char begin_table[]{ "\\begin{table}[h]" };
constexpr char end_table[]{ "\\end{table}" };
constexpr char end_tabular[]{ "\\end{tabular}" };

char const*
get_rowtype(const latex::RowType r)
{
  using latex::RowType;
  switch (r) {
    case RowType::CENTER:
      return " c ";
    case RowType::LEFT:
      return " l ";
    case RowType::RIGHT:
      return " r ";
    case RowType::CENTER_LINES:
      return "| c ";
    case RowType::LEFT_LINES:
      return "| l ";
    case RowType::RIGHT_LINES:
      return "| r ";
  }
}
QString
generate_table_header(const latex::ConfigOptions& config)
{
  QString tabular("\\begin{tabular}{");
  std::for_each(
    config.rowtypes.begin(), config.rowtypes.end(),
    [&tabular](const auto& rowtype) { tabular.append(get_rowtype(rowtype)); });

  if (config.rowtypes.last() >= latex::RowType::CENTER_LINES)
    tabular.append(" | ");

  tabular.append('}');
  return tabular;
}
}

QVector<QString>
latex::create_latex_from_grid_data(const QVector<QVector<QString>>& grid_data,
                                   const latex::ConfigOptions& config)
{
  QVector<QString> latex_output;
  latex_output.push_back(QString(begin_table));
  latex_output.push_back(generate_table_header(config));
  auto process_row = [&](const QVector<QString>& columns) {
    QString table_row;
    std::for_each(columns.begin(), columns.end(),
                  [&table_row](const auto& cell) {
                    table_row.append(QString("& %1").arg(cell));
                  });
    table_row.append("\\");
    return table_row;
  };

  std::for_each(grid_data.begin(), grid_data.end(), [&](const auto& row) {
    latex_output.push_back(process_row(row));
  });
  latex_output.push_back(QString(end_tabular));
  latex_output.push_back(QString(end_table));
  return latex_output;
}
