/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>
    Copyright 2011 Alex Richardson <alex.richardson@gmx.de>

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

#include "bytetableview.h"

// tool
#include "bytetabletool.h"
#include "bytetablemodel.h"
#include "bytetableviewsettings.h"
// KDE
#include <KPushButton>
#include <KLocale>
#include <KStandardGuiItem>
#include <KGlobalSettings>
//#include <KDebug>
#include <KApplication>
#include <KIntNumInput>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>


namespace Kasten2
{

ByteTableView::ByteTableView( ByteTableTool *tool, QWidget* parent )
  : AbstractToolWidget( parent ),
    mTool( tool )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    mByteTableView = new QTreeView( this );
    connect( KGlobalSettings::self(), SIGNAL(kdisplayFontChanged()),
             SLOT(setFixedFontByGlobalSettings()) );
    connect( KGlobalSettings::self(), SIGNAL(kdisplayFontChanged()),
             SLOT(resizeColumnsWidth()) );
    connect( KGlobalSettings::self(), SIGNAL(kdisplayStyleChanged()),
             SLOT(resizeColumnsWidth()) );
    setFixedFontByGlobalSettings(); //do this before setting model
    mByteTableView->setObjectName( QLatin1String( "ByteTable" ) );
    mByteTableView->setRootIsDecorated( false );
    mByteTableView->setItemsExpandable( false );
    mByteTableView->setUniformRowHeights( true );
    mByteTableView->setAllColumnsShowFocus( true );
    mByteTableView->setSortingEnabled( false );
    QHeaderView* header = mByteTableView->header();
    header->setFont( font() );
    header->setResizeMode( QHeaderView::Interactive );
    header->setStretchLastSection( false );
    mByteTableView->setModel( mTool->byteTableModel() );
    connect( mByteTableView, SIGNAL(doubleClicked(QModelIndex)),
             SLOT(onDoubleClicked(QModelIndex)) );

    baseLayout->addWidget( mByteTableView, 10 );

    QHBoxLayout *insertLayout = new QHBoxLayout();

    QLabel *label = new QLabel( i18nc("@label:spinbox number of bytes to insert","Number:"), this );
    insertLayout->addWidget( label );

    mInsertCountEdit = new KIntNumInput( this );
    mInsertCountEdit->setRange( 1, INT_MAX );
    mInsertCountEdit->setValue( 1 );
    mInsertCountEdit->setSuffix( ki18np(" byte"," bytes") );
    label->setBuddy( mInsertCountEdit );
    insertLayout->addWidget( mInsertCountEdit );
    const QString insertCountToolTip =
        i18nc( "@info:tooltip",
               "The number with which the byte currently selected in the table will be inserted." );
    label->setToolTip( insertCountToolTip );
    mInsertCountEdit->setToolTip( insertCountToolTip );

    insertLayout->addStretch();

    mInsertButton = new KPushButton( KStandardGuiItem::insert(), this );
    mInsertButton->setEnabled( mTool->hasWriteable() );
    connect( mTool, SIGNAL(hasWriteableChanged(bool)), mInsertButton, SLOT(setEnabled(bool)) );
    connect( mInsertButton, SIGNAL(clicked(bool)), SLOT(onInsertClicked()) );
    const QString insertButtonToolTip =
        i18nc( "@info:tooltip",
               "Inserts the byte currently selected in the table with the given number." );
    mInsertButton->setToolTip( insertButtonToolTip );
    addButton( mInsertButton, AbstractToolWidget::Default );
    insertLayout->addWidget( mInsertButton );

    baseLayout->addLayout( insertLayout );
    
    //if nothing has changed reuse the old values. This means the bytetable is fully constructed
    //after ~3ms and not 800 as it was before. If the saved values can not be reused it takes ~100ms
    const QList<int> columnsWidth = ByteTableViewSettings::columnsWidth();
    const QString styleName = KApplication::style()->objectName();
    const QString fixedFontData = KGlobalSettings::fixedFont().toString();
    if ( columnsWidth.size() < ByteTableModel::NoOfIds || styleName != ByteTableViewSettings::style()
            || fixedFontData != ByteTableViewSettings::fixedFont() )
    {
        resizeColumnsWidth();
    }
    else 
    {
        for (int i = 0; i < ByteTableModel::NoOfIds; ++i) 
        {
            header->resizeSection( i, columnsWidth.at( i ) );
        }
    }
}

void ByteTableView::resizeColumnsWidth()
{
    //kDebug() << "recalculating header width";
    QHeaderView* header = mByteTableView->header();
    for (int i = 0; i < ByteTableModel::NoOfIds; ++i) 
    {
        if ( i == ByteTableModel::CharacterId )
        {
            mByteTableView->resizeColumnToContents( i );
            continue;
        }
        //since all indexes in one row have same number of chars it is enough to calculate one row 
        //this speeds up calculating the width from 800ms to 100ms
        const QModelIndex index = mTool->byteTableModel()->index( 0, i );
        const int indexWidthHint = mByteTableView->sizeHintForIndex( index ).width();
        const int headerWidthHint = header->sectionSizeHint( i );
        header->resizeSection(i, qMax( indexWidthHint, headerWidthHint) );
    }
}


void ByteTableView::setFixedFontByGlobalSettings()
{
    mByteTableView->setFont( KGlobalSettings::fixedFont() );
}

void ByteTableView::onDoubleClicked( const QModelIndex &index )
{
    if( !mTool->hasWriteable() )
        return;

    const unsigned char byte = index.row();
    mTool->insert( byte, mInsertCountEdit->value() );
}

void ByteTableView::onInsertClicked()
{
    const unsigned char byte = mByteTableView->currentIndex().row();
    mTool->insert( byte, mInsertCountEdit->value() );
}

ByteTableView::~ByteTableView()
{
    QList<int> columnsWidth;
    const QHeaderView* header = mByteTableView->header();
    for (int i = 0 ; i < ByteTableModel::NoOfIds; ++i)
    {
        columnsWidth.append( header->sectionSize( i ) );
    }
    ByteTableViewSettings::setColumnsWidth( columnsWidth );
    ByteTableViewSettings::setStyle( KApplication::style()->objectName() );
    ByteTableViewSettings::setFixedFont( KGlobalSettings::fixedFont().toString() );
    ByteTableViewSettings::self()->writeConfig();
}

}
