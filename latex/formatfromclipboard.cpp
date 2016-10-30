#include "formatfromclipboard.h"
#include "cpp11-range/range.hpp"
#include "latexcreator.h"
#include <QDebug>
#include <QMimeData>
#include <QRegExp>
#include <algorithm> // count
#include <cassert>
#include <utility> // pair

using opt_tabledata = std::experimental::optional<QVector<QVector<QString>>>;

namespace {
opt_tabledata
generate_table_data(const QMimeData* mimedata)
{
  using util::lang::indices;

  // TODO: choose splitter based on a simple analyisis
  auto columnsplitter = QRegExp("[ \t]");
  auto clipboard_contents = mimedata->text();

  QStringList lines =
    clipboard_contents.split('\n', QString::SplitBehavior::SkipEmptyParts);
  QVector<QVector<QString>> data(lines.size());

  for (auto i : indices(data)) {
    auto cells = lines[i].split(columnsplitter);
    data[i] = QVector<QString>(cells.size());
    for (auto j : indices(data[i])) {
      data[i][j] = cells[j];
    }
    // return empty if dimensions unequal
    if (data[i].size() != data[0].size()) {
      return {};
    }
  }

  return data;
}
}

opt_tabledata
latex::grab_and_format_clipboard(not_null<const QMimeData*> clipboard)
{
  if (clipboard->hasText()) {
    qDebug() << clipboard->text();
    return generate_table_data(clipboard);
  }
  return {};
}
