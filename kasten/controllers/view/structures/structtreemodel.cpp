/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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
#include "structlogging.h"
#include "datatypes/datainformationwithchildren.h"
#include "datatypes/topleveldatainformation.h"
#include "datatypes/array/arraydatainformation.h"

#include <QFont>


namespace Kasten
{
StructTreeModel::StructTreeModel(StructTool* tool, QObject *parent) :
    QAbstractItemModel(parent), mTool(tool), mLastSender(nullptr), mLastStartIndex(0), mLastEndIndex(0)
{
    connect(mTool, &StructTool::dataChanged, this, &StructTreeModel::onToolDataChange);
    connect(mTool, &StructTool::dataCleared, this, &StructTreeModel::onToolDataClear);
    connect(mTool, &StructTool::childrenAboutToBeInserted,
            this, &StructTreeModel::onChildrenAboutToBeInserted);
    connect(mTool, &StructTool::childrenAboutToBeRemoved,
            this, &StructTreeModel::onChildrenAboutToBeRemoved);
    connect(mTool, &StructTool::childrenInserted,
            this, &StructTreeModel::onChildrenInserted);
    connect(mTool, &StructTool::childrenRemoved,
            this, &StructTreeModel::onChildrenRemoved);

}

StructTreeModel::~StructTreeModel()
{
}

void StructTreeModel::onChildrenRemoved(const DataInformation* sender, uint startIndex,
        uint endIndex)
{
    Q_ASSERT(sender == mLastSender);
    Q_ASSERT(startIndex== mLastStartIndex);
    Q_ASSERT(endIndex == mLastEndIndex);
    Q_UNUSED(sender)
    Q_UNUSED(startIndex)
    Q_UNUSED(endIndex)
    endRemoveRows();
}

void StructTreeModel::onChildrenInserted(const DataInformation* sender, uint startIndex,
        uint endIndex)
{
    Q_ASSERT(sender == mLastSender);
    Q_ASSERT(startIndex== mLastStartIndex);
    Q_ASSERT(endIndex == mLastEndIndex);
    Q_UNUSED(sender)
    Q_UNUSED(startIndex)
    Q_UNUSED(endIndex)
    endInsertRows();
}

void StructTreeModel::onChildrenAboutToBeRemoved(DataInformation* sender, uint startIndex,
        uint endIndex)
{
    //qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "data information" << sender->fullObjectPath() << ": removing "
    //        "children from index" << startIndex << "to" << endIndex;
    QModelIndex idx = findItemInModel(sender);
    Q_ASSERT(idx.isValid());
    mLastSender = sender;
    mLastStartIndex = startIndex;
    mLastEndIndex = endIndex;
    beginRemoveRows(idx, startIndex, endIndex);
}

void StructTreeModel::onChildrenAboutToBeInserted(DataInformation* sender, uint startIndex,
        uint endIndex)
{
    //qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "data information" << sender->fullObjectPath() << ": inserting "
    //        "children from index" << startIndex << "to" << endIndex;
    QModelIndex idx = findItemInModel(sender);
    Q_ASSERT(idx.isValid());
    mLastSender = sender;
    mLastStartIndex = startIndex;
    mLastEndIndex = endIndex;
    beginInsertRows(idx, startIndex, endIndex);
}

int StructTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return DataInformation::COLUMN_COUNT;
}

QVariant StructTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    DataInformation* item = static_cast<DataInformation*> (index.internalPointer());
    const int column = index.column();
    if (role == Qt::FontRole)
    {
        if (column == 0 && item->parent()->isTopLevel())
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
    if (item->parent()->isArray())
    {
        ArrayDataInformation* array = item->parent()->asArray();
        return array->childData(index.row(), column, role);
    }
    return item->data(column, role);
}

bool StructTreeModel::setData(const QModelIndex& index, const QVariant& value,
        int role)
{
    if (!index.isValid())
        return false;

    if (!index.internalPointer())
    {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "item == NULL";
        return false;
    }

    DataInformation* item = static_cast<DataInformation*> (index.internalPointer());
    bool change = mTool->setData(value, role, item, index.row());
    if (change)
    {
        emit dataChanged(index, index);
    }
    return change;
}

Qt::ItemFlags StructTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    DataInformation* item = static_cast<DataInformation*> (index.internalPointer());
    return item->flags(index.column(), mTool->isFileLoaded());
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

    DataInformation* childItem = nullptr;

    if (!parent.isValid())
        childItem = mTool->childAt(row);
    else
    {
        if (parent.column() != 0)
            return QModelIndex();
        DataInformation* parentItem = static_cast<DataInformation*> (parent.internalPointer());
        childItem = parentItem->childAt(row);
    }
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
        return QModelIndex();
}

QModelIndex StructTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    DataInformation* childItem = static_cast<DataInformation*> (index.internalPointer());

    DataInformationBase* parentObj = childItem->parent();

    if (!parentObj || parentObj->isTopLevel())
        return QModelIndex();

    // not null, not topleveldatainformation-> must be datainformation
    DataInformation* parent = parentObj->asDataInformation();
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
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "parentItem is NULL";
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

QModelIndex StructTreeModel::findItemInModel(DataInformationBase* data) const
{
    Q_CHECK_PTR(data);
    if (!data || data->isTopLevel())
        return QModelIndex(); //invalid object
    return createIndex(data->asDataInformation()->row(), 0, data);
}

void StructTreeModel::onToolDataChange(int row, void* data)
{
    emit dataChanged(createIndex(row, 0, data), createIndex(row, 2, data));
}

void StructTreeModel::onToolDataClear()
{
    beginResetModel();
    endResetModel();
}


}
