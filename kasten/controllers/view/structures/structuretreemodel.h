/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2011 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STRUCTURETREEMODEL_H
#define STRUCTURETREEMODEL_H

#include <QAbstractItemModel>
#include <QSet>

class DataInformationBase;
class DataInformation;

namespace Kasten
{
class StructuresTool;

class StructureTreeModel: public QAbstractItemModel
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
    //just for checking in debug mode:
    DataInformation* mLastSender;
    uint mLastStartIndex;
    uint mLastEndIndex;
};

}

#endif
