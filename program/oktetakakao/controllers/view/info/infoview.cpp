/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "infoview.h"

// tool
#include "infotool.h"
#include "statistictablemodel.h"
// KDE
#include <KPushButton>
#include <KGuiItem>
#include <KLocale>
#include <KGlobal>
#include <KGlobalSettings>
// Qt
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>


InfoView::InfoView( InfoTool *tool, QWidget *parent )
 : QWidget( parent ), mTool( tool )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    QHBoxLayout *updateLayout = new QHBoxLayout();

    updateLayout->addStretch();

    const KGuiItem updateGuiItem(
        i18nc("@action:button update the statistic of the byte frequency","&Update"),
        "view-refresh",
        i18nc("@info:tooltip","Updates the byte frequency statistics."),
        i18nc("@info:whatsthis",
              "If you press the <interface>Update</interface> button, the byte frequency statistics are updated.") );
    mUpdateButton = new KPushButton( updateGuiItem, this );
    mUpdateButton->setEnabled( mTool->isApplyable() );
    connect( mTool, SIGNAL(isApplyableChanged(bool)), mUpdateButton, SLOT( setEnabled(bool )) );
    connect( mUpdateButton, SIGNAL(clicked(bool)), mTool, SLOT(updateStatistic()) ); 
    updateLayout->addWidget( mUpdateButton );

    baseLayout->addLayout( updateLayout );

    QHBoxLayout *sizeLayout = new QHBoxLayout();

    QLabel *label = new QLabel( i18nc("@label size of selected bytes","Size:"), this );
    sizeLayout->addWidget( label );

    mSizeLabel = new QLabel( this );
    sizeLayout->addWidget( mSizeLabel, 10 );
    baseLayout->addLayout( sizeLayout );
    connect( mTool->statisticTableModel(), SIGNAL(sizeChanged( int )), SLOT(setByteArraySize( int )) );

    mStatisticTableView = new QTreeView( this );
    mStatisticTableView->setObjectName( "StatisticTable" );
    mStatisticTableView->setRootIsDecorated( false );
    mStatisticTableView->setItemsExpandable( false );
    mStatisticTableView->setUniformRowHeights( true );
    mStatisticTableView->setAllColumnsShowFocus( true );
    mStatisticTableView->setSortingEnabled( true );
    mStatisticTableView->setFont( KGlobalSettings::fixedFont() );
    QHeaderView* header = mStatisticTableView->header();
    header->setFont( font() );
    header->setResizeMode( QHeaderView::ResizeToContents );
    header->setStretchLastSection( false );
    // TODO: write subclass to filter count and percent by num, not string
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel( this );
    proxyModel->setDynamicSortFilter( true );
    proxyModel->setSourceModel( mTool->statisticTableModel() );
    mStatisticTableView->setModel( proxyModel );
    mStatisticTableView->sortByColumn( StatisticTableModel::CountId, Qt::DescendingOrder );
    connect( mTool->statisticTableModel(), SIGNAL(headerChanged()), SLOT(updateHeader()) );

    baseLayout->addWidget( mStatisticTableView, 10 );

    setByteArraySize( mTool->size() );
}

void InfoView::updateHeader()
{
    mStatisticTableView->resizeColumnToContents( StatisticTableModel::ValueId );
    mStatisticTableView->header()->headerDataChanged( Qt::Horizontal,
        StatisticTableModel::ValueId, StatisticTableModel::ValueId );
}

void InfoView::setByteArraySize( int size )
{
    const QString sizeText = ( size < 1 ) ? // -1 is default, 0 should not happen
        QString( '-' ) :
        i18np( "1 byte", "%1 bytes", size );

    mSizeLabel->setText( sizeText );
}

InfoView::~InfoView() {}
