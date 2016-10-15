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
QVector<QVector<QString>>
generate_table_data(const QMimeData* mimedata)
{
  using util::lang::indices;

  auto columnsplitter = QRegExp("[ \t]");
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
