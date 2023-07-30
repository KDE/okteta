/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "poddisplaymodel.hpp"

// tool
#include "podtablemodel.hpp"
#include "poddecodertool.hpp"
// libdatatypes
#include <binary8.hpp>
#include <octal8.hpp>
#include <hexadecimal8.hpp>
#include <float32.hpp>
#include <float64.hpp>
#include <char8.hpp>
#include <utf8.hpp>
#include <utf16.hpp>
#include <sint8.hpp>
#include <sint16.hpp>
#include <sint32.hpp>
#include <sint64.hpp>
#include <uint8.hpp>
#include <uint16.hpp>
#include <uint32.hpp>
#include <uint64.hpp>
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
        if (data.canConvert<Okteta::Binary8>()) {
            Okteta::Binary8 binary8 = data.value<Okteta::Binary8>();
            result = binary8.toString();
        } else if (data.canConvert<Okteta::Octal8>()) {
            Okteta::Octal8 octal8 = data.value<Okteta::Octal8>();
            result = octal8.toString();
        } else if (data.canConvert<Okteta::Hexadecimal8>()) {
            Okteta::Hexadecimal8 hexadecimal8 = data.value<Okteta::Hexadecimal8>();
            result = hexadecimal8.toString();
        } else if (data.canConvert<Okteta::SInt8>()) {
            Okteta::SInt8 sInt8 = data.value<Okteta::SInt8>();
            result = sInt8.toString(m_uiWidget->locale());
        } else if (data.canConvert<Okteta::SInt16>()) {
            Okteta::SInt16 sInt16 = data.value<Okteta::SInt16>();
            result = sInt16.toString(m_uiWidget->locale());
        } else if (data.canConvert<Okteta::SInt32>()) {
            Okteta::SInt32 sInt32 = data.value<Okteta::SInt32>();
            result = sInt32.toString(m_uiWidget->locale());
        } else if (data.canConvert<Okteta::SInt64>()) {
            Okteta::SInt64 sInt64 = data.value<Okteta::SInt64>();
            result = sInt64.toString(m_uiWidget->locale());
        } else if (data.canConvert<Okteta::UInt8>()) {
            Okteta::UInt8 uInt8 = data.value<Okteta::UInt8>();
            result = uInt8.toString(m_tool->isUnsignedAsHex(),m_uiWidget->locale());
        } else if (data.canConvert<Okteta::UInt16>()) {
            Okteta::UInt16 uInt16 = data.value<Okteta::UInt16>();
            result = uInt16.toString(m_tool->isUnsignedAsHex(), m_uiWidget->locale());
        } else if (data.canConvert<Okteta::UInt32>()) {
            Okteta::UInt32 uInt32 = data.value<Okteta::UInt32>();
            result = uInt32.toString(m_tool->isUnsignedAsHex(), m_uiWidget->locale());
        } else if (data.canConvert<Okteta::UInt64>()) {
            Okteta::UInt64 uInt64 = data.value<Okteta::UInt64>();
            result = uInt64.toString(m_tool->isUnsignedAsHex(), m_uiWidget->locale());
        } else if (data.canConvert<Okteta::Float32>()) {
            Okteta::Float32 float32 = data.value<Okteta::Float32>();
            result = float32.toString(m_uiWidget->locale());
        } else if (data.canConvert<Okteta::Float64>()) {
            Okteta::Float64 float64 = data.value<Okteta::Float64>();
            result = float64.toString(m_uiWidget->locale());
        } else if (data.canConvert<Okteta::Char8>()) {
            Okteta::Char8 char8 = data.value<Okteta::Char8>();
            result = char8.toString();
        } else if (data.canConvert<Okteta::Utf8>()) {
            Okteta::Utf8 utf8 = data.value<Okteta::Utf8>();
            result = utf8.toString();
        } else if (data.canConvert<Okteta::Utf16>()) {
            Okteta::Utf16 utf16 = data.value<Okteta::Utf16>();
            result = utf16.toString();
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
