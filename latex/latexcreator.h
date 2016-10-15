#pragma once
#include "../model/configoptions.h"
#include <QString>
#include <QVector>

namespace latex {
QVector<QString> create_latex_from_grid_data(
  const QVector<QVector<QString>>& grid_data,
  const latex::ConfigOptions& config);
QString flatten_latex(const QVector<QString>& generated_latex);
}
