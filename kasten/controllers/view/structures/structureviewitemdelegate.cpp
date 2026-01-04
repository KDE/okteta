/*
    SPDX-FileCopyrightText: 2009, 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structureviewitemdelegate.hpp"

// tool
#include "structuretreemodel.hpp"
#include <datainformation.hpp>

namespace Kasten {

StructureViewItemDelegate::StructureViewItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

StructureViewItemDelegate::~StructureViewItemDelegate() = default;

QWidget* StructureViewItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                                                 const QModelIndex& index) const
{
    Q_UNUSED(option)
    if (!index.isValid()) {
        return nullptr;
    }
    auto* const data = index.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    if (!data) {
        return nullptr;
    }
    QWidget* const ret = data->createEditWidget(parent);
    ret->setFocusPolicy(Qt::WheelFocus);
    return ret;
}

void StructureViewItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                             const QModelIndex& index) const
{
    if (!index.isValid()) {
        return;
    }
    const auto* const data = index.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    if (!data) {
        return;
    }
    QVariant value = data->dataFromWidget(editor);
    // editor might have intermediate edit state, will return invalid QVariant
    if (value.isValid()) {
        model->setData(index, value, Qt::EditRole);
    }
}

void StructureViewItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{

    if (!index.isValid()) {
        return;
    }
    auto* const data = index.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    if (!data) {
        return;
    }
    data->setWidgetData(editor);
}

}

#include "moc_structureviewitemdelegate.cpp"
