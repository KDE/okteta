/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "containedstringtablemodel.hpp"

// KF
#include <KLocalizedString>

ContainedStringTableModel::ContainedStringTableModel(const QList<ContainedString>* containedStringList,
                                                     int offsetCoding,
                                                     QObject* parent)
    : QAbstractTableModel(parent)
    , mContainedStringList(containedStringList)
{
    mPrintFunction = Okteta::OffsetFormat::printFunction((Okteta::OffsetFormat::Format)offsetCoding);
}

ContainedStringTableModel::~ContainedStringTableModel() = default;

void ContainedStringTableModel::setOffsetCoding(int offsetCoding)
{
    mPrintFunction = Okteta::OffsetFormat::printFunction((Okteta::OffsetFormat::Format)offsetCoding);
    beginResetModel();
    endResetModel();
}

void ContainedStringTableModel::update()
{
    beginResetModel();
    endResetModel();
}

int ContainedStringTableModel::rowCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? mContainedStringList->size() : 0;
}

int ContainedStringTableModel::columnCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? NoOfColumnIds : 0;
}

QVariant ContainedStringTableModel::data(const QModelIndex& index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole
        || role == Qt::ToolTipRole) {
        const int stringIndex = index.row();

        if (0 <= stringIndex && stringIndex < mContainedStringList->size()) {
            const ContainedString& string = mContainedStringList->at(stringIndex);

            const int column = index.column();
            switch (column)
            {
            case OffsetColumnId:
            {
                if (role == Qt::DisplayRole) {
                    mPrintFunction(mCodedOffset, string.offset());

                    result = QString::fromLatin1(mCodedOffset);
                }
                break;
            }
            case StringColumnId:
            {
                result = string.string();
                break;
            }
            default:
                ;
            }
        }
    }

    return result;
}

QVariant ContainedStringTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if (role == Qt::DisplayRole) {
        const QString titel =
            section == OffsetColumnId ? i18nc("@title:column offset of the extracted string",       "Offset") :
            section == StringColumnId ? i18nc("@title:column string extracted from the byte array", "String") :
                                        QString();
        result = titel;
    } else {
        result = QAbstractTableModel::headerData(section, orientation, role);
    }

    return result;
}
