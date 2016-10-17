#pragma once
#include "GSL/gsl/gsl"
#include <QClipboard>
#include <QString>
#include <QVector>
#include <experimental/optional>

using gsl::not_null;

namespace latex {
std::experimental::optional<QVector<QVector<QString>>>
grab_and_format_clipboard(not_null<const QClipboard*> clipboard);
}
