/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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
#include "infoviewsettings.h"
// KDE
#include <KPushButton>
#include <KGuiItem>
#include <KLocale>
#include <KGlobal>
#include <KApplication>
#include <KGlobalSettings>
// Qt
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>
#include <QtGui/QFontMetrics>


namespace Kasten2
{

InfoView::InfoView( InfoTool *tool, QWidget* parent )
 : QWidget( parent ), mTool( tool )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    QHBoxLayout* topLineLayout = new QHBoxLayout();

    QLabel *label = new QLabel( i18nc("@label size of selected bytes","Size:"), this );
    topLineLayout->addWidget( label );

    mSizeLabel = new QLabel( this );
    const QString sizeToolTip =
        i18nc( "@info:tooltip",
               "The number of the bytes the statistic was built for." );
    label->setToolTip( sizeToolTip );
    mSizeLabel->setToolTip( sizeToolTip );
    topLineLayout->addWidget( mSizeLabel, 10 );
    connect( mTool->statisticTableModel(), SIGNAL(sizeChanged(int)),
             SLOT(setByteArraySize(int)) );

    topLineLayout->addStretch();

    const KGuiItem updateGuiItem =
        KGuiItem(i18nc("@action:button build the statistic of the byte frequency",
                       "&Build"),
                 QLatin1String("run-build"),
                 i18nc("@info:tooltip",
                       "Builds the byte frequency statistic for the bytes in the selected range."),
                 i18nc("@info:whatsthis",
                       "If you press the <interface>Build</interface> button,"
                       " the byte frequency statistic is built for the bytes in the selected range.") );
    mUpdateButton = new KPushButton( updateGuiItem, this );
    mUpdateButton->setEnabled( mTool->isApplyable() );
    connect( mTool, SIGNAL(isApplyableChanged(bool)), mUpdateButton, SLOT(setEnabled(bool)) );
    connect( mUpdateButton, SIGNAL(clicked(bool)), mTool, SLOT(updateStatistic()) ); 
    topLineLayout->addWidget( mUpdateButton );

    baseLayout->addLayout( topLineLayout );

    mStatisticTableView = new QTreeView( this );
    connect( KGlobalSettings::self(), SIGNAL(kdisplayFontChanged()),
             SLOT(setFixedFontByGlobalSettings()) );
    connect( KGlobalSettings::self(), SIGNAL(kdisplayFontChanged()),
             SLOT(resizeColumnsWidth()) );
    connect( KGlobalSettings::self(), SIGNAL(kdisplayStyleChanged()),
             SLOT(resizeColumnsWidth()) );
    setFixedFontByGlobalSettings(); //do this before setting model
    mStatisticTableView->setObjectName( QLatin1String( "StatisticTable" ) );
    mStatisticTableView->setRootIsDecorated( false );
    mStatisticTableView->setItemsExpandable( false );
    mStatisticTableView->setUniformRowHeights( true );
    mStatisticTableView->setAllColumnsShowFocus( true );
    mStatisticTableView->setSortingEnabled( true );
    QHeaderView* header = mStatisticTableView->header();
    header->setFont( font() );
    header->setResizeMode( QHeaderView::Interactive );
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
    
    //if nothing has changed reuse the old values. This means the info view is fully constructed much quicker.
    const QList<int> columnsWidth = InfoViewSettings::columnsWidth();
    const QString styleName = KApplication::style()->objectName();
    const QString fixedFontData = KGlobalSettings::fixedFont().toString();
    if ( columnsWidth.size() < StatisticTableModel::NoOfIds || styleName != InfoViewSettings::style()
            || fixedFontData != InfoViewSettings::fixedFont() )
    {
        resizeColumnsWidth();
    }
    else 
    {
        for (int i = 0; i < StatisticTableModel::NoOfIds; ++i) 
        {
            header->resizeSection( i, columnsWidth.at( i ) );
        }
    }
}

void InfoView::updateHeader()
{
    mStatisticTableView->resizeColumnToContents( StatisticTableModel::ValueId );
    mStatisticTableView->header()->headerDataChanged( Qt::Horizontal,
        StatisticTableModel::ValueId, StatisticTableModel::ValueId );
}

void InfoView::resizeColumnsWidth()
{
    //kDebug() << "recalculating header width";
    for (int i = 0; i < StatisticTableModel::NoOfIds; ++i) 
    {
        mStatisticTableView->resizeColumnToContents( i );
    }
}

void InfoView::setByteArraySize( int size )
{
    const QString sizeText = ( size < 1 ) ? // -1 is default, 0 should not happen
        QString::fromLatin1( "-" ) :
        i18np( "1 byte", "%1 bytes", size );

    mSizeLabel->setText( sizeText );
}

void InfoView::setFixedFontByGlobalSettings()
{
    mStatisticTableView->setFont( KGlobalSettings::fixedFont() );
}

InfoView::~InfoView() 
{
    QList<int> columnsWidth;
    const QHeaderView* header = mStatisticTableView->header();
    for (int i = 0 ; i < StatisticTableModel::NoOfIds; ++i)
    {
        columnsWidth.append( header->sectionSize( i ) );
    }
    InfoViewSettings::setColumnsWidth( columnsWidth );
    InfoViewSettings::setStyle( KApplication::style()->objectName() );
    InfoViewSettings::setFixedFont( KGlobalSettings::fixedFont().toString() );
    InfoViewSettings::self()->writeConfig();
}

}
