#pragma once
#include <QClipboard>
#include <QString>
#include <QVector>
namespace latex {
QVector<QVector<QString>> grab_and_format_clipboard(const QClipboard* mimedata);
}
