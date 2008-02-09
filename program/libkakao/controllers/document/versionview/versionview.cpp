/***************************************************************************
                          versionview.cpp  -  description
                             -------------------
    begin                : Sat Feb 9 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "versionview.h"

// tool
#include "versiontablemodel.h"
// Kakao core
#include <kiversionable.h>
#include <kabstractdocument.h>
// KDE
#include <KDialog>
#include <KLocale>
// Qt
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QLayout>
#include <QtGui/QTreeView>


VersionView::VersionView( QWidget *parent )
 : QWidget( parent )
{
    mVersionTableModel = new VersionTableModel( 0, 0, this );

    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );
    baseLayout->setSpacing( KDialog::spacingHint() );

    mVersionTableView = new QTreeView( this );
    mVersionTableView->setObjectName( "VersionsTable" );
    mVersionTableView->setRootIsDecorated( false );
    mVersionTableView->setItemsExpandable( false );
    mVersionTableView->setUniformRowHeights( true );
    mVersionTableView->setAllColumnsShowFocus( true );
    mVersionTableView->setSortingEnabled( true );
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel( this );
    proxyModel->setSourceModel( mVersionTableModel );
    mVersionTableView->setModel( proxyModel );
    mVersionTableView->sortByColumn( VersionTableModel::IdColumnId, Qt::AscendingOrder );

    baseLayout->addWidget( mVersionTableView, 10 );
}

void VersionView::setDocument( KAbstractDocument *document )
{
    KDE::If::Versionable *versionControl = document ? qobject_cast<KDE::If::Versionable*>( document ) : 0;

    mVersionTableModel->setDocument( document, versionControl );

    for( int c = 0; c<VersionTableModel::NoOfColumnIds; ++c )
        mVersionTableView->resizeColumnToContents( c );
}


VersionView::~VersionView() {}
