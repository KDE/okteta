/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
// KDE
#include <KPushButton>
#include <KLocale>
#include <KStandardGuiItem>
#include <KGlobalSettings>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <KIntNumInput>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>


namespace Kasten
{

ByteTableView::ByteTableView( ByteTableTool *tool, QWidget* parent )
  : AbstractToolWidget( parent ),
    mTool( tool )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    mByteTableView = new QTreeView( this );
    mByteTableView->setObjectName( "ByteTable" );
    mByteTableView->setRootIsDecorated( false );
    mByteTableView->setItemsExpandable( false );
    mByteTableView->setUniformRowHeights( true );
    mByteTableView->setAllColumnsShowFocus( true );
    mByteTableView->setSortingEnabled( false );
    QHeaderView* header = mByteTableView->header();
    header->setFont( font() );
    header->setResizeMode( QHeaderView::ResizeToContents );
    header->setStretchLastSection( false );
    mByteTableView->setModel( mTool->byteTableModel() );
    connect( mByteTableView, SIGNAL(doubleClicked( const QModelIndex& )),
             SLOT(onDoubleClicked( const QModelIndex& )) );
    connect( KGlobalSettings::self(), SIGNAL(kdisplayFontChanged()),
             SLOT(setFixedFontByGlobalSettings()) );
    setFixedFontByGlobalSettings();

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
    connect( mTool, SIGNAL(hasWriteableChanged(bool)), mInsertButton, SLOT( setEnabled(bool )) );
    connect( mInsertButton, SIGNAL(clicked(bool)), SLOT(onInsertClicked()) );
    const QString insertButtonToolTip =
        i18nc( "@info:tooltip",
               "Inserts the byte currently selected in the table with the given number." );
    mInsertButton->setToolTip( insertButtonToolTip );
    addButton( mInsertButton, AbstractToolWidget::Default );
    insertLayout->addWidget( mInsertButton );

    baseLayout->addLayout( insertLayout );
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

ByteTableView::~ByteTableView() {}

}
