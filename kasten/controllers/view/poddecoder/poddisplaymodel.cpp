/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "poddisplaymodel.hpp"

// tool
#include "podtablemodel.hpp"
#include "poddecodertool.hpp"
// libdatatypedisplaystringifiers
#include <abstractdatatypedisplaystringifier.hpp>
// Qt
#include <QWidget>
#include <QLocale>
#include <QMimeData>

namespace Kasten {

PODDisplayModel::PODDisplayModel(QWidget* uiWidget, PODDecoderTool* tool, QObject* parent)
    : QIdentityProxyModel(parent)
    , m_uiWidget(uiWidget)
    , m_tool(tool)
{
}

PODDisplayModel::~PODDisplayModel() = default;


QVariant PODDisplayModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole) {
        return QIdentityProxyModel::data(index, role);
    }

    QVariant result;

    const int column = index.column();
    const QVariant data = sourceModel()->data(index, Qt::DisplayRole);

    switch (column)
    {
    case PODTableModel::ValueId: {
        Okteta::AbstractDataTypeDisplayStringifier* stringifier = m_dataTypeDisplayStringifierManager.stringifier(data.userType());
        if (stringifier) {
            result = stringifier->displayText(data, m_uiWidget->locale(), m_tool->isUnsignedAsHex());
            break;
        }
        [[fallthrough]];
    }
    default:
        result = data;
    }

    return result;
}

Qt::ItemFlags PODDisplayModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QIdentityProxyModel::flags(index);

    const int podId = index.row();
    if (!m_tool->value(podId).isNull()) {
        flags |= Qt::ItemIsDragEnabled;
    }

    return flags;
}

QMimeData* PODDisplayModel::mimeData(const QModelIndexList& indexes) const
{
    if (indexes.isEmpty()) {
        return nullptr;
    }

    const QModelIndex index = indexes.first().siblingAtColumn(PODTableModel::ValueId);

    auto mimeData = std::make_unique<QMimeData>();

    const QString displayText = index.data(Qt::DisplayRole).toString();
    mimeData->setText(displayText);
    // TODO: query via role from index instead?
    const int podId = index.row();
    mimeData->setData(QStringLiteral("application/octet-stream"), m_tool->bytes(podId));

    return mimeData.release();
}

QStringList PODDisplayModel::mimeTypes() const
{
    return {
        QStringLiteral("application/octet-stream"),
        QStringLiteral("text/plain"),
    };
}

}

#include "moc_poddisplaymodel.cpp"
