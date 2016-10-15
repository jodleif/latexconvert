#include "formatfromclipboard.h"
#include "cpp11-range/range.hpp"
#include "latexcreator.h"
#include <QDebug>
#include <QMimeData>
#include <QRegExp>
#include <algorithm> // count
#include <cassert>
#include <utility> // pair
namespace {
std::pair<int, int>
get_new_table_dimensions(QString contents)
{
  int amount_of_spaces = std::count(contents.begin(), contents.end(), ' ');
  int amount_of_lines = std::count(contents.begin(), contents.end(), '\n');
  int rows = amount_of_lines + 1;
  int cols = (amount_of_spaces / rows) + rows;
  return { rows, cols };
}

QVector<QVector<QString>>
generate_table_data(const QMimeData* mimedata)
{
  auto columnsplitter = QRegExp("[ \t]");
  using util::lang::indices;
  auto clipboard_contents = mimedata->text();
  QStringList lines = clipboard_contents.split('\n');
  QVector<QVector<QString>> data(lines.size());
  for (auto i : indices(data)) {
    auto cells = lines[i].split(columnsplitter);
    data[i] = QVector<QString>(cells.size());
    for (auto j : indices(data[i])) {
      data[i][j] = cells[j];
    }
  }
  auto line_iter = lines.begin();
  std::for_each(data.begin(), data.end(),
                [](const auto& ele) { qDebug() << ele; });
  return data;
}
}

QVector<QVector<QString>>
latex::grab_and_format_clipboard(const QClipboard* clipboard)
{
  const auto* mimedata = clipboard->mimeData();
  if (mimedata->hasText()) {
    qDebug() << mimedata->text();
    return generate_table_data(mimedata);
  }
  return QVector<QVector<QString>>();
}
