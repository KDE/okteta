/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "statisticdisplaymodel.hpp"

// tool
#include "statistictablemodel.hpp"
// Qt
#include <QWidget>
#include <QLocale>
#include <QMimeData>

namespace Kasten {

StatisticDisplayModel::StatisticDisplayModel(QWidget* uiWidget, QObject* parent)
    : QIdentityProxyModel(parent)
    , m_uiWidget(uiWidget)
{
}

StatisticDisplayModel::~StatisticDisplayModel() = default;


QVariant StatisticDisplayModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QIdentityProxyModel::data(index, role);
    }

    QVariant result;

    const int column = index.column();
    const QVariant data = sourceModel()->data(index, Qt::DisplayRole);

    switch (column)
    {
    case StatisticTableModel::PercentId: {
        result = data.isValid() ?
                QVariant(m_uiWidget->locale().toString(data.toDouble(), 'f', 6)) :
                QVariant(QStringLiteral("-"));
        break;
    }
    default:
        result = data;
    }

    return result;
}

Qt::ItemFlags StatisticDisplayModel::flags(const QModelIndex& index) const
{
    return QIdentityProxyModel::flags(index) | Qt::ItemIsDragEnabled;
}

static QString tableAsText(const QModelIndexList& indexes)
{
    QString lines;

    // assumes indexes are in row-wise order as seen on screen
    // add tab between items in row, but for last linefeed
    int lastRow = -1;
    for (const QModelIndex& index : indexes) {
        const int row = index.row();
        if (row != lastRow) {
            lastRow = row;
            if (!lines.isEmpty()) {
                lines.append(QLatin1Char('\n'));
            }
        } else {
            lines.append(QLatin1Char('\t'));
        }

        lines.append(index.data(Qt::DisplayRole).toString());
    }

    lines.append(QLatin1Char('\n'));

    return lines;
}

QMimeData* StatisticDisplayModel::mimeData(const QModelIndexList& indexes) const
{
    if (indexes.isEmpty()) {
        return nullptr;
    }

    auto* mimeData = new QMimeData;

    const QString text = tableAsText(indexes);
    mimeData->setText(text);

    return mimeData;
}

QStringList StatisticDisplayModel::mimeTypes() const
{
    return {
        QStringLiteral("text/plain"),
    };
}

}

#include "moc_statisticdisplaymodel.cpp"
