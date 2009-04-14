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

#include "versionview.h"

// tool
#include "versiontablemodel.h"
#include "versionviewtool.h"
// Kakao core
#include <kiversionable.h>
#include <abstractmodel.h>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QTreeView>


VersionView::VersionView( VersionViewTool* tool, QWidget* parent )
 : QWidget( parent ), mTool( tool )
{
    mVersionTableModel = new VersionTableModel( 0, 0, this );

    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    mVersionTableView = new QTreeView( this );
    mVersionTableView->setObjectName( "VersionsTable" );
    mVersionTableView->setRootIsDecorated( false );
    mVersionTableView->setItemsExpandable( false );
    mVersionTableView->setUniformRowHeights( true );
    mVersionTableView->setAllColumnsShowFocus( true );
    mVersionTableView->setModel( mVersionTableModel );

    baseLayout->addWidget( mVersionTableView, 10 );

    connect( mTool, SIGNAL(modelChanged( AbstractModel* )),
             SLOT(setModel( AbstractModel* )) );

    setModel( mTool->model() );
}

void VersionView::setModel( AbstractModel* model )
{
    KDE::If::Versionable* versionControl = model ? qobject_cast<KDE::If::Versionable*>( model ) : 0;

    mVersionTableModel->setModel( model, versionControl );

    for( int c = 0; c<VersionTableModel::NoOfColumnIds; ++c )
        mVersionTableView->resizeColumnToContents( c );
}

VersionView::~VersionView() {}
