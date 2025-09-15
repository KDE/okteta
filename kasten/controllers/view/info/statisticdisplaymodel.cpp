/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "statisticdisplaymodel.hpp"

// tool
#include "oktetakastengui_export.hpp"
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

static QString tableAsHtml(const QModelIndexList& indexes)
{
    QString html;

    if (!indexes.isEmpty()) {
        // assumes indexes are in row-wise order as seen on screen
        // wrap each items in row with <td>,
        // but for last add <tr> end & start, matched by global start / end wrapper
        int lastRow = indexes.first().row();
        for (const QModelIndex &index : indexes) {
            const int row = index.row();
            if (row != lastRow) {
                lastRow = row;
                if (!html.isEmpty()) {
                    html.append(QLatin1String("</tr><tr>"));
                }
            }

            html.append(QLatin1String("<td>") + index.data(Qt::DisplayRole).toString() + QLatin1String("</td>"));
        }
    }

    html = QLatin1String("<html><head><meta name=\"generator\" content=\"Okteta Statistics Tool " OKTETAKASTENGUI_VERSION_STRING "\"></head><table><tr>") +  html + QLatin1String("</tr></table></html>");

    return html;
}

QMimeData* StatisticDisplayModel::mimeData(const QModelIndexList& indexes) const
{
    if (indexes.isEmpty()) {
        return nullptr;
    }

    auto mimeData = std::make_unique<QMimeData>();

    const QString text = tableAsText(indexes);
    mimeData->setText(text);
    const QString html = tableAsHtml(indexes);
    mimeData->setHtml(html);

    return mimeData.release();
}

QStringList StatisticDisplayModel::mimeTypes() const
{
    return {
        QStringLiteral("text/plain"),
        QStringLiteral("text/html"),
    };
}

}

#include "moc_statisticdisplaymodel.cpp"
