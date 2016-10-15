#pragma once
#include <QVector>
namespace latex {
enum class RowType
{
  CENTER,
  LEFT,
  RIGHT,
  CENTER_LINES,
  LEFT_LINES,
  RIGHT_LINES
};

struct ConfigOptions
{
  int rows;
  int columns;
  QVector<RowType> rowtypes;
};

ConfigOptions generate_config_options(int rows, int columns,
                                      RowType rowtype = RowType::CENTER);
}
