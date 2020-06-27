/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTURETREEMODEL_HPP
#define KASTEN_STRUCTURETREEMODEL_HPP

#include <QAbstractItemModel>
#include <QSet>

class DataInformationBase;
class DataInformation;

namespace Kasten {
class StructuresTool;

class StructureTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit StructureTreeModel(StructuresTool* tool, QObject* parent = nullptr);
    ~StructureTreeModel() override;

public: // QAbstractItemModel API
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;

private:
    QModelIndex findItemInModel(DataInformationBase* data) const;

private Q_SLOTS:
    void onToolDataChange(int row, void* data);
    void onToolDataClear();
    void onChildrenAboutToBeRemoved(DataInformation* sender, uint startIndex, uint endIndex);
    void onChildrenAboutToBeInserted(DataInformation* sender, uint startIndex, uint endIndex);
    void onChildrenRemoved(const DataInformation* sender, uint startIndex, uint endIndex);
    void onChildrenInserted(const DataInformation* sender, uint startIndex, uint endIndex);

private:
    StructuresTool* mTool;
    // just for checking in debug mode:
    DataInformation* mLastSender;
    uint mLastStartIndex;
    uint mLastEndIndex;
};

}

#endif
