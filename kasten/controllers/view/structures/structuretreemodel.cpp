/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuretreemodel.hpp"
#include "structurestool.hpp"
#include "structlogging.hpp"
#include "datatypes/datainformationwithchildren.hpp"
#include "datatypes/topleveldatainformation.hpp"
#include "datatypes/array/arraydatainformation.hpp"

#include <QFont>

namespace Kasten {

StructureTreeModel::StructureTreeModel(StructuresTool* tool, QObject* parent)
    : QAbstractItemModel(parent)
    , mTool(tool)
    , mLastSender(nullptr)
    , mLastStartIndex(0)
    , mLastEndIndex(0)
{
    connect(mTool, &StructuresTool::dataChanged, this, &StructureTreeModel::onToolDataChange);
    connect(mTool, &StructuresTool::dataCleared, this, &StructureTreeModel::onToolDataClear);
    connect(mTool, &StructuresTool::childrenAboutToBeInserted,
            this, &StructureTreeModel::onChildrenAboutToBeInserted);
    connect(mTool, &StructuresTool::childrenAboutToBeRemoved,
            this, &StructureTreeModel::onChildrenAboutToBeRemoved);
    connect(mTool, &StructuresTool::childrenInserted,
            this, &StructureTreeModel::onChildrenInserted);
    connect(mTool, &StructuresTool::childrenRemoved,
            this, &StructureTreeModel::onChildrenRemoved);

}

StructureTreeModel::~StructureTreeModel() = default;

void StructureTreeModel::onChildrenRemoved(const DataInformation* sender, uint startIndex,
                                           uint endIndex)
{
    Q_ASSERT(sender == mLastSender);
    Q_ASSERT(startIndex == mLastStartIndex);
    Q_ASSERT(endIndex == mLastEndIndex);
    Q_UNUSED(sender)
    Q_UNUSED(startIndex)
    Q_UNUSED(endIndex)
    endRemoveRows();
}

void StructureTreeModel::onChildrenInserted(const DataInformation* sender, uint startIndex,
                                            uint endIndex)
{
    Q_ASSERT(sender == mLastSender);
    Q_ASSERT(startIndex == mLastStartIndex);
    Q_ASSERT(endIndex == mLastEndIndex);
    Q_UNUSED(sender)
    Q_UNUSED(startIndex)
    Q_UNUSED(endIndex)
    endInsertRows();
}

void StructureTreeModel::onChildrenAboutToBeRemoved(DataInformation* sender, uint startIndex,
                                                    uint endIndex)
{
    // qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "data information" << sender->fullObjectPath() << ": removing "
    //        "children from index" << startIndex << "to" << endIndex;
    QModelIndex idx = findItemInModel(sender);
    Q_ASSERT(idx.isValid());
    mLastSender = sender;
    mLastStartIndex = startIndex;
    mLastEndIndex = endIndex;
    beginRemoveRows(idx, startIndex, endIndex);
}

void StructureTreeModel::onChildrenAboutToBeInserted(DataInformation* sender, uint startIndex,
                                                     uint endIndex)
{
    // qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "data information" << sender->fullObjectPath() << ": inserting "
    //        "children from index" << startIndex << "to" << endIndex;
    QModelIndex idx = findItemInModel(sender);
    Q_ASSERT(idx.isValid());
    mLastSender = sender;
    mLastStartIndex = startIndex;
    mLastEndIndex = endIndex;
    beginInsertRows(idx, startIndex, endIndex);
}

int StructureTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return DataInformation::COLUMN_COUNT;
}

QVariant StructureTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    auto* item = static_cast<DataInformation*> (index.internalPointer());
    const int column = index.column();
    if (role == Qt::FontRole) {
        if (column == 0 && item->parent()->isTopLevel()) {
            // TODO: ideally here we would not take the default application font
            // (as given by QFont()) but the default of the view
            QFont font;
            font.setBold(true);
            return font;
        }
        return {};
    }
    if (item->parent()->isArray()) {
        ArrayDataInformation* array = item->parent()->asArray();
        return array->childData(index.row(), column, role);
    }
    return item->data(column, role);
}

bool StructureTreeModel::setData(const QModelIndex& index, const QVariant& value,
                                 int role)
{
    if (!index.isValid()) {
        return false;
    }

    if (!index.internalPointer()) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "item == NULL";
        return false;
    }

    auto* item = static_cast<DataInformation*> (index.internalPointer());
    bool change = mTool->setData(value, role, item, index.row());
    if (change) {
        emit dataChanged(index, index);
    }
    return change;
}

Qt::ItemFlags StructureTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    auto* item = static_cast<DataInformation*> (index.internalPointer());
    return item->flags(index.column(), mTool->isFileLoaded());
}

QVariant StructureTreeModel::headerData(int section, Qt::Orientation orientation,
                                        int role) const
{
    if (orientation == Qt::Horizontal) {
        return mTool->headerData(section, role);
    }
    return {};
}

QModelIndex StructureTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) {
        return {};
    }

    DataInformation* childItem = nullptr;

    if (!parent.isValid()) {
        childItem = mTool->childAt(row);
    } else {
        if (parent.column() != 0) {
            return {};
        }
        auto* parentItem = static_cast<DataInformation*> (parent.internalPointer());
        childItem = parentItem->childAt(row);
    }
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return {};
}

QModelIndex StructureTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return {};
    }

    auto* childItem = static_cast<DataInformation*> (index.internalPointer());

    DataInformationBase* parentObj = childItem->parent();

    if (!parentObj || parentObj->isTopLevel()) {
        return {};
    }

    // not null, not topleveldatainformation-> must be datainformation
    DataInformation* parent = parentObj->asDataInformation();
    return createIndex(parent->row(), 0, parent);
}

int StructureTreeModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid()) {
        return mTool->childCount();
    }
    if (parent.column() != 0) {
        return 0;
    }
    auto* parentItem = static_cast<DataInformation*> (parent.internalPointer());
    if (!parentItem) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "parentItem is NULL";
        return mTool->childCount();
    }
    return parentItem->childCount();
}

bool StructureTreeModel::hasChildren(const QModelIndex& parent) const
{
    if (!parent.isValid()) {
        return mTool->childCount() > 0;
    }
    auto* parentItem = static_cast<DataInformation*> (parent.internalPointer());
    if (!parentItem) {
        return false;
    }
    return parentItem->childCount() > 0;
}

QModelIndex StructureTreeModel::findItemInModel(DataInformationBase* data) const
{
    Q_CHECK_PTR(data);
    if (!data || data->isTopLevel()) {
        return {}; // invalid object
    }
    return createIndex(data->asDataInformation()->row(), 0, data);
}

void StructureTreeModel::onToolDataChange(int row, void* data)
{
    emit dataChanged(createIndex(row, 0, data), createIndex(row, 2, data));
}

void StructureTreeModel::onToolDataClear()
{
    beginResetModel();
    endResetModel();
}

}
