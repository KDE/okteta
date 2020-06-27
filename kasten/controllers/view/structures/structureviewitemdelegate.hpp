/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREVIEWITEMDELEGATE_HPP
#define KASTEN_STRUCTUREVIEWITEMDELEGATE_HPP

#include <QStyledItemDelegate>

class StructureViewItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit StructureViewItemDelegate(QObject* parent = nullptr);
    ~StructureViewItemDelegate() override;

public: // QStyledItemDelegate API
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
};

#endif
