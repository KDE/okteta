/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CONTAINEDSTRINGTABLEMODEL_HPP
#define KASTEN_CONTAINEDSTRINGTABLEMODEL_HPP

// tool
#include "containedstring.hpp"
// Okteta gui
#include <Okteta/OffsetFormat>
// Qt
#include <QList>
#include <QAbstractTableModel>

class ContainedStringTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnIds
    {
        OffsetColumnId = 0,
        StringColumnId = 1,
        NoOfColumnIds = 2 // TODO: what pattern is usually used to mark number of ids?
    };

public:
    ContainedStringTableModel(const QList<ContainedString>* containedStringList, int offsetCoding,
                              QObject* parent = nullptr);
    ~ContainedStringTableModel() override;

public: // QAbstractTableModel API
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public Q_SLOTS:
    void update();
    void setOffsetCoding(int offsetCoding);

private:
    const QList<ContainedString>* const mContainedStringList;

    Okteta::OffsetFormat::print mPrintFunction;
    mutable char mCodedOffset[Okteta::OffsetFormat::MaxFormatWidth + 1];
};

#endif
