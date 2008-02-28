/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "infoview.h"

// tool
#include "infotool.h"
#include "statistictablemodel.h"
// KDE
#include <KPushButton>
#include <KDialog>
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
    baseLayout->setSpacing( KDialog::spacingHint() );

    QHBoxLayout *sizeLayout = new QHBoxLayout();

    QLabel *label = new QLabel( i18n("Size [bytes]: "), this );
    sizeLayout->addWidget( label );

    mSizeLabel = new QLabel( this );
    sizeLayout->addWidget( mSizeLabel );
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
    mStatisticTableView->header()->setFont( font() );
    // TODO: write subclass to filter count and percent by num, not string
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel( this );
    proxyModel->setDynamicSortFilter( true );
    proxyModel->setSourceModel( mTool->statisticTableModel() );
    mStatisticTableView->setModel( proxyModel );
    for( int c = 0; c<StatisticTableModel::NoOfIds; ++c )
        mStatisticTableView->resizeColumnToContents( c );
    mStatisticTableView->sortByColumn( StatisticTableModel::CountId, Qt::DescendingOrder );
    connect( mTool->statisticTableModel(), SIGNAL(headerChanged()), SLOT(updateHeader()) );

    baseLayout->addWidget( mStatisticTableView, 10 );

    QHBoxLayout *updateLayout = new QHBoxLayout();

    mDirtyLabel = new QLabel( this );
    updateLayout->addWidget( mDirtyLabel );
    connect( mTool, SIGNAL(statisticDirty( bool )), SLOT(setDirty( bool )) );
    updateLayout->addStretch();

    mUpdateButton = new KPushButton( i18n("Update"), this );
    mUpdateButton->setEnabled( mTool->hasByteArrayView() );
    connect( mTool, SIGNAL(byteArrayViewChanged(bool)), mUpdateButton, SLOT( setEnabled(bool )) );
    connect( mUpdateButton, SIGNAL(clicked(bool)), mTool, SLOT(updateStatistic()) ); 
    updateLayout->addWidget( mUpdateButton );

    baseLayout->addLayout( updateLayout );
}

void InfoView::updateHeader()
{
    mStatisticTableView->resizeColumnToContents( StatisticTableModel::ValueId );
    mStatisticTableView->header()->headerDataChanged( Qt::Horizontal,
        StatisticTableModel::ValueId, StatisticTableModel::ValueId );
}

void InfoView::setByteArraySize( int size )
{
    // TODO: use "-" if there is no byte array!
    mSizeLabel->setText( KGlobal::locale()->formatNumber(size, 0) );
}

void InfoView::setDirty( bool dirty )
{
    mDirtyLabel->setText( dirty ? i18nc("the statistic is not uptodate","Warning: not uptodate") : QString() );
    mDirtyLabel->setToolTip( dirty ? i18n("Warning: Byte Array has been modified since last update.") : QString() );
}

InfoView::~InfoView() {}
