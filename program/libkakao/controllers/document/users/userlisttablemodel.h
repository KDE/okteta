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

#ifndef USERLISTTABLEMODEL_H
#define USERLISTTABLEMODEL_H


// Qt
#include <QtCore/QAbstractTableModel>
#include <QtCore/QList>

namespace KDE { namespace If {
class UserListable;
}}
class AbstractModel;

class UserListTableModel : public QAbstractTableModel
{
  Q_OBJECT

  public:
    enum ColumnIds
    {
        UserColumnId = 0,
        NoOfColumnIds = 1 // TODO: what pattern is usually used to mark number of ids?
    };

    enum Roles
    {
        FaceIconRole = Qt::UserRole,
        ConnectionRole
    };

  public:
    UserListTableModel( AbstractModel* model, KDE::If::UserListable* userListControl, QObject* parent = 0 );
    virtual ~UserListTableModel();

  public: // QAbstractTableModel API
    virtual int rowCount( const QModelIndex& parent ) const;
    virtual int columnCount( const QModelIndex& parent ) const;
    virtual QVariant data( const QModelIndex& index, int role ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

  public:
    void setModel( AbstractModel* model, KDE::If::UserListable* userListControl );

  protected Q_SLOTS:
    void onUserListChanged();

  protected:
    AbstractModel* mModel;
    KDE::If::UserListable* mUserListControl;

    QList<int> mUserList;
};

#endif
