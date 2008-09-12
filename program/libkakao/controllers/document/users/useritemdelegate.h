/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef USERITEMDELEGATE_H
#define USERITEMDELEGATE_H

// Qt
#include <QtGui/QItemDelegate>

class UserItemDelegate : public QItemDelegate
{
  public:
    UserItemDelegate( QObject* parent = 0 );

  public: // QAbstractItemDelegate API
    virtual void paint( QPainter* painter,
                        const QStyleOptionViewItem& option,
                        const QModelIndex& index ) const;
    virtual QSize sizeHint( const QStyleOptionViewItem& option,
                            const QModelIndex& index ) const;

  protected:
};

#endif
