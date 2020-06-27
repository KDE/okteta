/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PODTABLEMODEL_HPP
#define KASTEN_PODTABLEMODEL_HPP

// Qt
#include <QAbstractTableModel>

namespace Kasten {

class PODDecoderTool;

class PODTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnIds
    {
        NameId = 0,
        ValueId = 1,
        // UsedBytes = x,  TODO: add hint how many bytes a datatype uses
        NoOfColumnIds = 2 // TODO: what pattern is usually used to mark number of ids?
    };

public:
    explicit PODTableModel(PODDecoderTool* tool, QObject* parent = nullptr);
    ~PODTableModel() override;

public: // QAbstractItemModel API
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QModelIndex buddy(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

private Q_SLOTS:
    void onDataChanged();

private:
    PODDecoderTool* mTool;
    const QString mEmptyNote;
};

}

#endif
