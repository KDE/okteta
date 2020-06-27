/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "podtablemodel.hpp"

// lib
#include "poddecodertool.hpp"
// KF
#include <KLocalizedString>
#include <KColorScheme>
// Qt
#include <QApplication>

namespace Kasten {

PODTableModel::PODTableModel(PODDecoderTool* tool, QObject* parent)
    : QAbstractTableModel(parent)
    , mTool(tool)
    , mEmptyNote(QLatin1Char('-'))
{
    connect(mTool, &PODDecoderTool::dataChanged, this, &PODTableModel::onDataChanged);
}

PODTableModel::~PODTableModel() = default;

void PODTableModel::onDataChanged()
{
    emit dataChanged(index(0, ValueId), index(mTool->podCount() - 1, ValueId));
}

int PODTableModel::rowCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? mTool->podCount() : 0;
}

int PODTableModel::columnCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? NoOfColumnIds : 0;
}

QVariant PODTableModel::data(const QModelIndex& index, int role) const
{
    QVariant result;
    switch (role)
    {
    case Qt::DisplayRole:
    {
        const int podId = index.row();
        const int column = index.column();
        switch (column)
        {
        case NameId:
        {
            result = mTool->nameOfPOD(podId);
            break;
        }
        case ValueId:
        {
            QVariant value = mTool->value(podId);
            if (value.isNull()) {
                value = mEmptyNote;
            }
            result = value;
            break;
        }
        default:
            ;
        }
        break;
    }
    case Qt::EditRole:
    {
        const int column = index.column();
        if (column == ValueId) {
            const int podId = index.row();
            result = mTool->value(podId);
        }
        break;
    }
    case Qt::TextAlignmentRole:
    {
        const int column = index.column();
        result = (column == NameId) ? Qt::AlignRight : Qt::AlignLeft;
        break;
    }
    case Qt::ForegroundRole:
    {
        const int column = index.column();
        if (column == ValueId) {
            const int podId = index.row();
            const QVariant value = mTool->value(podId);

            if (value.isNull()) {
                const QPalette& palette = QApplication::palette();
                const KColorScheme colorScheme(palette.currentColorGroup(), KColorScheme::View);
                result = colorScheme.foreground(KColorScheme::InactiveText);
            }
        }
    }
    default:
        break;
    }

    return result;
}

Qt::ItemFlags PODTableModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags result = QAbstractTableModel::flags(index);
    const int column = index.column();
    if (column == ValueId) {
        const int podId = index.row();
        const QVariant value = mTool->value(podId);

        // TODO: this check does not match types with dynamic byte length, e.g. utf-8!
        if (!value.isNull()) {
            result |= Qt::ItemIsEditable;
        }
    }

    return result;
}

QModelIndex PODTableModel::buddy(const QModelIndex& index) const
{
    QModelIndex result;

    const int column = index.column();
    if (column == NameId) {
        const int row = index.row();
        result = createIndex(row, ValueId);
    } else {
        result = index;
    }

    return result;
}

QVariant PODTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if (role == Qt::DisplayRole) {
        const QString titel =
            section == NameId ?   i18nc("@title:column name of the datatype",                "Type") :
            section == ValueId ?  i18nc("@title:column value of the bytes for the datatype", "Value") :
                                  QString();
        result = titel;
    } else if (role == Qt::ToolTipRole) {
        const QString titel =
            section == NameId ?
                i18nc("@info:tooltip for column Type",    "The type of data") :
            section == ValueId ?
                i18nc("@info:tooltip for column Value",   "The value of the bytes for the datatype") :
                QString();
        result = titel;
    } else {
        result = QAbstractTableModel::headerData(section, orientation, role);
    }

    return result;
}

bool PODTableModel::setData(const QModelIndex& index, const QVariant& data, int role)
{
    bool result;

    if (index.isValid() && role == Qt::EditRole) {
        const int podId = index.row();

        mTool->setData(data, podId);

        result = true;
    } else {
        result = false;
    }

    return result;
}

}
