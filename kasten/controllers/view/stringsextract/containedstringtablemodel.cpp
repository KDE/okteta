/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "containedstringtablemodel.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QFontDatabase>
#include <QMimeData>

namespace Kasten {

ContainedStringTableModel::ContainedStringTableModel(const QList<ContainedString>* containedStringList,
                                                     int offsetCoding,
                                                     QObject* parent)
    : QAbstractTableModel(parent)
    , mContainedStringList(containedStringList)
    , mFixedFont(QFontDatabase::systemFont(QFontDatabase::FixedFont))
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
    } else if (role == Qt::FontRole) {
        result = mFixedFont;
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

Qt::ItemFlags ContainedStringTableModel::flags(const QModelIndex& index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsDragEnabled;
}

static QString stringColumnAsText(const QModelIndexList& indexes)
{
    QString strings;

    // assumes indexes are in row-wise order as seen on screen
    // add tab between items in row, but for last linefeed
    int lastRow = -1;
    for (const QModelIndex& index : indexes) {
        const int row = index.row();
        if (row == lastRow) {
            continue;
        }
        lastRow = row;
        const QModelIndex stringIndex = index.siblingAtColumn(ContainedStringTableModel::StringColumnId);
        strings.append(stringIndex.data(Qt::DisplayRole).toString());
        strings.append(QLatin1Char('\n')); // TODO: specific linefeed for platforms
    }

    return strings;
}

QMimeData* ContainedStringTableModel::mimeData(const QModelIndexList& indexes) const
{
    if (indexes.isEmpty()) {
        return nullptr;
    }

    auto mimeData = std::make_unique<QMimeData>();

    const QString text = stringColumnAsText(indexes);
    mimeData->setText(text);

    return mimeData.release();
}

QStringList ContainedStringTableModel::mimeTypes() const
{
    return {
        QStringLiteral("text/plain"),
    };
}

}

#include "moc_containedstringtablemodel.cpp"
