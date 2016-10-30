#pragma once
#include "GSL/gsl/gsl"
#include <QString>
#include <QVector>
#include <experimental/optional>

using gsl::not_null;
class QMimeData;

namespace latex {
std::experimental::optional<QVector<QVector<QString>>>
grab_and_format_clipboard(not_null<const QMimeData*> clipboard);
}
