#include "tablemodel.h"
#include <QVector>
#include <algorithm>
#include <cassert>
#include <functional>

void
TableModel::reserve_data()
{
  if (grid_data.size() == 0) {
    grid_data.reserve(rows);
    for (int i{ 0 }; i < rows; ++i) {
      grid_data.push_back(QVector<QString>(columns)); // Default initialize
    }
  }
}

TableModel::TableModel(QObject* parent)
  : QAbstractTableModel(parent)
{
  rows = 0;
  columns = 0;
}

TableModel::TableModel(QObject* parent, int rows, int cols,
                       QVector<QVector<QString>>&& grid_data)
  : QAbstractTableModel(parent)
  , rows(rows)
  , columns(cols)
  , grid_data(std::move(grid_data))
{
  reserve_data();
}

int
TableModel::rowCount(const QModelIndex& /*parent*/) const
{
  return rows;
}

int
TableModel::columnCount(const QModelIndex& /*parent*/) const
{
  return columns;
}

QVariant
TableModel::data(const QModelIndex& index, int role) const
{
  if (role == Qt::DisplayRole) {
    assert(index.row() >= 0 && index.row() < rows);
    assert(index.column() >= 0 && index.column() < columns);
    return QString(grid_data[index.row()][index.column()]);
  }
  return QVariant();
}

bool
TableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (role == Qt::EditRole) {
    int c{ index.column() };
    int r{ index.row() };

    assert(c >= 0 && c < columns);
    assert(r >= 0 && r < rows);

    grid_data[r][c] = value.toString();

    emit editCompleted(grid_data[c][r]);
  }
  return true;
}

Qt::ItemFlags
TableModel::flags(const QModelIndex& /*index*/) const
{
  return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVector<QVector<QString>>
TableModel::get_grid_data() const
{
  return grid_data;
}

const QVector<QVector<QString>>&
TableModel::get_c_grid_data() const
{
  return std::cref(grid_data);
}
/**
 * @brief addColumn add one extra column for each row.
 * @param grid_data
 * @return
 */
QVector<QVector<QString>>
tablemodel::addColumn(QVector<QVector<QString>> grid_data)
{
  std::for_each(grid_data.begin(), grid_data.end(),
                [](auto& column) { column.push_back(QString()); });
  return grid_data;
}

QVector<QVector<QString>>
tablemodel::addRow(QVector<QVector<QString>> grid_data)
{
  assert(grid_data.size() > 0);
  auto new_row = QVector<QString>(grid_data[0].size());
  grid_data.append(new_row);

  return grid_data;
}

QVector<QVector<QString>>
tablemodel::removeRow(QVector<QVector<QString>> grid_data)
{
  assert(grid_data.size() > 0);
#ifndef NDEBUG
  int size = grid_data.size();
#endif
  grid_data.pop_back();
  assert(size - 1 == grid_data.size());
  return grid_data;
}

QVector<QVector<QString>>
tablemodel::removeColumn(QVector<QVector<QString>> grid_data)
{
  std::for_each(grid_data.begin(), grid_data.end(), [](auto& vec) {
#ifndef NDEBUG
    int size = vec.size();
#endif
    vec.pop_back();
    assert(vec.size() == size - 1);
  });
  return grid_data;
}
