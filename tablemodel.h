#pragma once
#include <QAbstractTableModel>

namespace tablemodel {
QVector<QVector<QString>> addColumn(QVector<QVector<QString>> grid_data);
QVector<QVector<QString>> addRow(QVector<QVector<QString>> grid_data);
QVector<QVector<QString>> removeColumn(QVector<QVector<QString>> grid_data);
QVector<QVector<QString>> removeRow(QVector<QVector<QString>> grid_data);
}

class TableModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit TableModel(QObject* parent = nullptr);
  explicit TableModel(
    QObject* parent = nullptr, int rows = 0, int cols = 0,
    QVector<QVector<QString>>&& grid_data = QVector<QVector<QString>>());
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex& index, const QVariant& value,
               int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex& index) const;
  QVector<QVector<QString>> get_grid_data() const;

private:
  int rows;
  int columns;
  QVector<QVector<QString>> grid_data;

  void reserve_data();
signals:
  void editCompleted(const QString& str);
};
