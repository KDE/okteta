/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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

#include "structviewitemdelegate.h"
#include <QLineEdit>
#include <KDebug>

StructViewItemDelegate::StructViewItemDelegate(QObject * parent) :
    QStyledItemDelegate(parent)
{
}

StructViewItemDelegate::~StructViewItemDelegate()
{
}

void StructViewItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * model,
        const QModelIndex & index) const
{
    QLineEdit* lineedit = dynamic_cast<QLineEdit*> (editor);
    if (lineedit)
    {
        model->setData(index, lineedit->text(), Qt::EditRole);
    }
    else
        kDebug() << "dyn cast failed -> wrong edit widget";
}

void StructViewItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    QLineEdit* lineedit = dynamic_cast<QLineEdit*> (editor);
    if (lineedit)
    {
        lineedit->clear();
        lineedit->setText(index.data(Qt::EditRole).toString());
    }
    else
        kDebug() << "dyn cast failed -> wrong edit widget";
}
QString StructViewItemDelegate::displayText(const QVariant & value, const QLocale&) const
{
    return value.toString();
}

