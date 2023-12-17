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

}

#include "moc_statisticdisplaymodel.cpp"
