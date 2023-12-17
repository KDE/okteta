/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "poddisplaymodel.hpp"

// tool
#include "podtablemodel.hpp"
#include "poddecodertool.hpp"
#include "types/binary8.hpp"
#include "types/octal8.hpp"
#include "types/hexadecimal8.hpp"
#include "types/float32.hpp"
#include "types/float64.hpp"
#include "types/char8.hpp"
#include "types/utf8.hpp"
#include "types/sint8.hpp"
#include "types/sint16.hpp"
#include "types/sint32.hpp"
#include "types/sint64.hpp"
#include "types/uint8.hpp"
#include "types/uint16.hpp"
#include "types/uint32.hpp"
#include "types/uint64.hpp"
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
    case PODTableModel::ValueId:
        if (data.canConvert<Binary8>()) {
            Binary8 binary8 = data.value<Binary8>();
            result = binary8.toString();
        } else if (data.canConvert<Octal8>()) {
            Octal8 octal8 = data.value<Octal8>();
            result = octal8.toString();
        } else if (data.canConvert<Hexadecimal8>()) {
            Hexadecimal8 hexadecimal8 = data.value<Hexadecimal8>();
            result = hexadecimal8.toString();
        } else if (data.canConvert<SInt8>()) {
            SInt8 sInt8 = data.value<SInt8>();
            result = sInt8.toString(m_uiWidget->locale());
        } else if (data.canConvert<SInt16>()) {
            SInt16 sInt16 = data.value<SInt16>();
            result = sInt16.toString(m_uiWidget->locale());
        } else if (data.canConvert<SInt32>()) {
            SInt32 sInt32 = data.value<SInt32>();
            result = sInt32.toString(m_uiWidget->locale());
        } else if (data.canConvert<SInt64>()) {
            SInt64 sInt64 = data.value<SInt64>();
            result = sInt64.toString(m_uiWidget->locale());
        } else if (data.canConvert<UInt8>()) {
            UInt8 uInt8 = data.value<UInt8>();
            result = uInt8.toString(m_tool->isUnsignedAsHex(),m_uiWidget->locale());
        } else if (data.canConvert<UInt16>()) {
            UInt16 uInt16 = data.value<UInt16>();
            result = uInt16.toString(m_tool->isUnsignedAsHex(), m_uiWidget->locale());
        } else if (data.canConvert<UInt32>()) {
            UInt32 uInt32 = data.value<UInt32>();
            result = uInt32.toString(m_tool->isUnsignedAsHex(), m_uiWidget->locale());
        } else if (data.canConvert<UInt64>()) {
            UInt64 uInt64 = data.value<UInt64>();
            result = uInt64.toString(m_tool->isUnsignedAsHex(), m_uiWidget->locale());
        } else if (data.canConvert<Float32>()) {
            Float32 float32 = data.value<Float32>();
            result = float32.toString(m_uiWidget->locale());
        } else if (data.canConvert<Float64>()) {
            Float64 float64 = data.value<Float64>();
            result = float64.toString(m_uiWidget->locale());
        } else if (data.canConvert<Char8>()) {
            Char8 char8 = data.value<Char8>();
            result = char8.toString();
        } else if (data.canConvert<Utf8>()) {
            Utf8 utf8 = data.value<Utf8>();
            result = utf8.toString();
        } else {
            result = data;
        }
        break;
    default:
        result = data;
    }

    return result;
}

Qt::ItemFlags PODDisplayModel::flags(const QModelIndex& index) const
{
    return QIdentityProxyModel::flags(index) | Qt::ItemIsDragEnabled;
}

QMimeData* PODDisplayModel::mimeData(const QModelIndexList& indexes) const
{
    if (indexes.isEmpty()) {
        return nullptr;
    }

    const QModelIndex index = indexes.first().siblingAtColumn(PODTableModel::ValueId);

    auto* mimeData = new QMimeData;

    const QString displayText = index.data(Qt::DisplayRole).toString();
    mimeData->setText(displayText);
    // TODO: query via role from index instead?
    const int podId = index.row();
    mimeData->setData(QStringLiteral("application/octet-stream"), m_tool->bytes(podId));

    return mimeData;
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
