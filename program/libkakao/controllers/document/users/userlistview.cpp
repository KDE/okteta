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

#include "userlistview.h"

// tool
#include "useritemdelegate.h"
#include "userlisttablemodel.h"
#include "userscontroltool.h"
// Kakao core
#include <userlistable.h>
#include <abstractmodel.h>
// KDE
#include <KDialog>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QTreeView>


UserListView::UserListView( UsersControlTool* tool, QWidget* parent )
 : QWidget( parent ), mTool( tool )
{
    mUserListTableModel = new UserListTableModel( 0, 0, this );

    QVBoxLayout* baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );
    baseLayout->setSpacing( KDialog::spacingHint() );

    mUserListTableView = new QTreeView( this );
    mUserListTableView->setObjectName( "UsersTable" );
    mUserListTableView->setRootIsDecorated( false );
    mUserListTableView->setItemsExpandable( false );
    mUserListTableView->setUniformRowHeights( true );
    mUserListTableView->setAllColumnsShowFocus( true );
    mUserListTableView->setModel( mUserListTableModel );
    mUserListTableView->setItemDelegate(new UserItemDelegate(this) );
    mUserListTableView->setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );

    baseLayout->addWidget( mUserListTableView, 10 );

    connect( mTool, SIGNAL(modelChanged( AbstractModel* )),
             SLOT(setModel( AbstractModel* )) );

    setModel( mTool->model() );
}

void UserListView::setModel( AbstractModel* model )
{
    KDE::If::UserListable* userListControl = model ? qobject_cast<KDE::If::UserListable*>( model ) : 0;

    mUserListTableModel->setModel( model, userListControl );

//     for( int c = 0; c<UserListTableModel::NoOfColumnIds; ++c )
//         mUserListTableView->resizeColumnToContents( c );
}

UserListView::~UserListView() {}
