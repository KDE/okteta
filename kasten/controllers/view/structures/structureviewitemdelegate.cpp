/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structureviewitemdelegate.hpp"
#include "datatypes/datainformation.hpp"

#include <QScopedPointer>

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
    auto* data = static_cast<DataInformation*> (index.internalPointer());
    if (!data) {
        return nullptr;
    }
    QWidget* ret = data->createEditWidget(parent);
    ret->setFocusPolicy(Qt::WheelFocus);
    return ret;
}

void StructureViewItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                             const QModelIndex& index) const
{
    if (!index.isValid()) {
        return;
    }
    auto* data = static_cast<DataInformation*> (index.internalPointer());
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
    auto* data = static_cast<DataInformation*> (index.internalPointer());
    if (!data) {
        return;
    }
    data->setWidgetData(editor);
}

#include "moc_structureviewitemdelegate.cpp"
