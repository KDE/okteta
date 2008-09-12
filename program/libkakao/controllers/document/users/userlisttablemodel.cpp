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

#include "userlisttablemodel.h"

// Kakao core
#include <userlistable.h>
#include <abstractmodel.h>
#include <person.h>
// KDE
#include <KLocale>
#include <KIcon>

UserListTableModel::UserListTableModel( AbstractModel* model, KDE::If::UserListable* userListControl,
                                                    QObject* parent )
 : QAbstractTableModel( parent ),
   mModel( 0 )
{
    setModel( model, userListControl );
}

void UserListTableModel::setModel( AbstractModel* model, KDE::If::UserListable* userListControl )
{
    if( mModel ) mModel->disconnect( this );

    mModel = model;

    mUserListControl = userListControl;

    if( mModel )
    {
        connect( mModel, SIGNAL(usersAdded( const QList<Person>& )), SLOT(onUserListChanged()) );
        connect( mModel, SIGNAL(usersRemoved( const QList<Person>& )), SLOT(onUserListChanged()) );
    }

    reset();
}

int UserListTableModel::rowCount( const QModelIndex& parent ) const
{
Q_UNUSED( parent )
    return mUserListControl ? mUserListControl->userList().count() : 0;
}

int UserListTableModel::columnCount( const QModelIndex& parent ) const
{
Q_UNUSED( parent )
    return NoOfColumnIds;
}

QVariant UserListTableModel::data( const QModelIndex& index, int role ) const
{
    QVariant result;

    const int versionIndex = index.row();
    const Person person = mUserListControl->userList().at( versionIndex );
    switch( role )
    {
    case Qt::DisplayRole:
    {
        result = person.name();
        break;
    }
    case FaceIconRole: {
        result = person.faceIcon().pixmap(48,48);
        break;
    }
    case ConnectionRole:
    {
        const QString connection = "ProcessDocuments";
        result = connection;
        break;
    }
    }

    return result;
}

QVariant UserListTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant result;
#if 0
    if( role == Qt::DisplayRole )
    {
        const QString titel =
            section == IdColumnId ?                i18nc("@title:column Id of the version",         "Id") :
            section == ChangeDescriptionColumnId ? i18nc("@title:column description of the change", "Changes") :
            QString();
        result = titel;
    }
    else if( role == Qt::ToolTipRole )
    {
        const QString titel =
            section == IdColumnId ?                i18nc("@info:tooltip","Id of the version") :
            section == ChangeDescriptionColumnId ? i18nc("@info:tooltip","Description of what changed") :
            QString();
        result = titel;
    }
    else
        result = QAbstractTableModel::headerData( section, orientation, role );
#endif
    return result;
}
#if 0
void UserListTableModel::onRevertedToVersionIndex( int versionIndex )
{
    if( mVersionIndex == versionIndex )
        return;

    const int oldVersionIndex = mVersionIndex;
    mVersionIndex = versionIndex;

    emit dataChanged( index(versionIndex,CurrentColumnId), index(versionIndex,CurrentColumnId) );
    emit dataChanged( index(oldVersionIndex,CurrentColumnId), index(oldVersionIndex,CurrentColumnId) );
}
#endif

void UserListTableModel::onUserListChanged()
{
    reset();
}

UserListTableModel::~UserListTableModel() {}
