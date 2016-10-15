#include "configoptions.h"
#include <algorithm>

latex::ConfigOptions
latex::generate_config_options(int rows, int columns, latex::RowType rowtype)
{
  latex::ConfigOptions config;
  config.rows = rows;
  config.columns = columns;
  auto rowtypes = QVector<latex::RowType>(columns);

  std::for_each(rowtypes.begin(), rowtypes.end(),
                [rowtype](auto& row) { row = rowtype; });

  config.rowtypes = rowtypes;
  return config;
}
