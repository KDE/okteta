/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "structtreemodel.h"
#include "structtool.h"
#include "datatypes/datainformationwithchildren.h"
#include "datatypes/topleveldatainformation.h"
#include "datatypes/abstractarraydatainformation.h"

namespace Kasten
{
StructTreeModel::StructTreeModel(StructTool* tool, QObject *parent) :
    QAbstractItemModel(parent), mTool(tool)
{
    connect(mTool, SIGNAL(dataChanged(int, void*)), this, SLOT(onToolDataChange(int, void*)));
    connect(mTool, SIGNAL(dataCleared()), this, SLOT(onToolDataClear()));
}

StructTreeModel::~StructTreeModel()
{
}

void StructTreeModel::onChildrenRemoved(const QObject* sender, uint startIndex,
        uint endIndex)
{
    kDebug()
        << "data information " << sender->objectName() << ": removed " << (endIndex
                - startIndex + 1) << " children starting at offset " << startIndex;
    emit endRemoveRows();
}

void StructTreeModel::onChildrenInserted(const QObject* sender, uint startIndex,
        uint endIndex)
{
    kDebug()
        << "data information " << sender->objectName() << ": inserted " << (endIndex
                - startIndex + 1) << " children at offset " << startIndex;
    emit endInsertRows();
}

void StructTreeModel::onChildrenAboutToBeRemoved(QObject* sender, uint startIndex,
        uint endIndex)
{
    kDebug()
        << "data information " << sender->objectName()
                << ": about to remove children:" << startIndex << " to " << endIndex;
    QModelIndex idx = findItemInModel(sender);
    Q_ASSERT(idx.isValid());
    emit beginRemoveRows(idx, startIndex, endIndex);
}

void StructTreeModel::onChildrenAboutToBeInserted(QObject* sender, uint startIndex,
        uint endIndex)
{
    kDebug()
        << "data information " << sender->objectName()
                << ": about to insert children:" << startIndex << " to " << endIndex;
    QModelIndex idx = findItemInModel(sender);
    Q_ASSERT(idx.isValid());
    emit beginInsertRows(idx, startIndex, endIndex);
}

int StructTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mTool->columnCount();
}

QVariant StructTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    DataInformation* item = static_cast<DataInformation*> (index.internalPointer());
    const int column = index.column();
    if (role == Qt::FontRole)
    {
        if (column == 0 && (item->parent() == 0
                || dynamic_cast<TopLevelDataInformation*> (item->parent())))
        {
            // TODO: ideally here we would not take the default application font
            // (as given by QFont()) but the default of the view
            QFont font;
            font.setBold(true);
            return font;
        }
        else
            return QVariant();
    }
    
    //const QModelIndex& parent = index.parent();
    //if (parent.isValid())
    //{
    const AbstractArrayDataInformation* array =
        dynamic_cast<const AbstractArrayDataInformation*>(item->parent());
    if (array)
    {
        return array->childData(index.row(), column, role);
    }
    //}
    else
        return item->data(column, role);
}
bool StructTreeModel::setData(const QModelIndex& index, const QVariant& value,
        int role)
{
    if (!index.isValid())
        return false;

    if (!index.internalPointer())
    {
        kDebug()
            << "item == NULL";
        return false;
    }

    DataInformation* item = static_cast<DataInformation*> (index.internalPointer());
    bool change = mTool->setData(value, role, item);
    if (change)
    {
        emit dataChanged(index, index);
    }
    return change;
}

Qt::ItemFlags StructTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;
    DataInformation *item = static_cast<DataInformation*> (index.internalPointer());
    return mTool->flags(index.column(), item);
}

QVariant StructTreeModel::headerData(int section, Qt::Orientation orientation,
        int role) const
{
    if (orientation == Qt::Horizontal)
    {
        return mTool->headerData(section, role);
    }
    return QVariant();
}

QModelIndex StructTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DataInformation* childItem = NULL;

    if (!parent.isValid())
        childItem = mTool->childAt(row);
    else
    {
        if (parent.column() != 0)
            return QModelIndex();
        DataInformation* parentItem =
                static_cast<DataInformation*> (parent.internalPointer());
        childItem = parentItem->childAt(row);
    }
    if (childItem)
    {
        if (dynamic_cast<DataInformationWithChildren*> (childItem))
        {
            DataInformationWithChildren* chldItm =
                    static_cast<DataInformationWithChildren*> (childItem);
            //assume that all items with children can change their childCount
            if (!mItemsWithSignalConnected.contains(chldItm))
            {
                //                kDebug()
                //                    << "connecting '" << childItem->name()
                //                            << "'s childCountChanged signal to model";
                //only connect once
                mItemsWithSignalConnected.insert(chldItm);
                connect(childItem, SIGNAL(destroyed(QObject*)),
                        SLOT(removeItemFromSignalsList(QObject*))); //remove after was deleted

                connect(childItem,
                        SIGNAL(childrenAboutToBeRemoved(QObject*, uint, uint)),
                        this, SLOT(onChildrenAboutToBeRemoved(QObject*, uint, uint)));
                connect(childItem,
                        SIGNAL(childrenAboutToBeInserted(QObject*, uint, uint)),
                        this,
                        SLOT(onChildrenAboutToBeInserted(QObject*, uint, uint)));
                //also connect the done signals
                connect(childItem,
                        SIGNAL(childrenRemoved(const QObject*, uint, uint)), this,
                        SLOT(onChildrenRemoved(const QObject*, uint, uint)));
                connect(childItem,
                        SIGNAL(childrenInserted(const QObject*, uint, uint)), this,
                        SLOT(onChildrenInserted(const QObject*, uint, uint)));
            }
        }
        return createIndex(row, column, childItem);
    }
    else
        return QModelIndex();
}

QModelIndex StructTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    DataInformation *childItem =
            static_cast<DataInformation*> (index.internalPointer());

    if (!childItem->parent())
        return QModelIndex();

    QObject* parentObj = static_cast<QObject*> (childItem->parent());

    if (!parentObj)
        return QModelIndex();
    if (dynamic_cast<TopLevelDataInformation*> (parentObj))
    {
        //parent is a TopLevelDataInformation -> not valid
        return QModelIndex();
    }

    // not null, not topleveldatainformation-> must be dataninformation
    DataInformation* parent = static_cast<DataInformation*> (parentObj);
    return createIndex(parent->row(), 0, parent);
}

int StructTreeModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return mTool->childCount();
    if (parent.column() != 0)
        return 0;
    DataInformation* parentItem =
            static_cast<DataInformation*> (parent.internalPointer());
    if (!parentItem)
    {
        kDebug()
            << "parentItem is NULL";
        return mTool->childCount();
    }
    return parentItem->childCount();
}

bool StructTreeModel::hasChildren(const QModelIndex& parent) const
{
    if (!parent.isValid())
        return mTool->childCount() > 0;
    DataInformation* parentItem =
            static_cast<DataInformation*> (parent.internalPointer());
    if (!parentItem)
        return false;
    else
        return parentItem->childCount() > 0;
}

void StructTreeModel::removeItemFromSignalsList(QObject* obj)
{
    //obj has been deleted -> remove from list
    mItemsWithSignalConnected.remove(
            dynamic_cast<DataInformationWithChildren*> (obj));
}

QModelIndex StructTreeModel::findItemInModel(QObject* obj) const
{
    DataInformation* data = dynamic_cast<DataInformation*> (obj);
    Q_CHECK_PTR(data);
    if (!data)
        return QModelIndex(); //invalid object
    return createIndex(data->row(), 0, obj);
}

void StructTreeModel::onToolDataChange(int row, void* data)
{
    emit dataChanged(createIndex(row, 0, data), createIndex(row, 2, data));
}


}
