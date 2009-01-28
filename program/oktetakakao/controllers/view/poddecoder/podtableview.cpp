/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "podtableview.h"

// controller
#include "podtablemodel.h"
#include "poddecodertool.h"
// KDE
#include <KComboBox>
#include <KDialog>
#include <KLocale>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QTreeView>
#include <QtGui/QHeaderView>


PODTableView::PODTableView( PODDecoderTool* tool, QWidget* parent )
 : QWidget( parent ), mTool( tool )
{
    QBoxLayout* baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );//KDialog::marginHint() );
    baseLayout->setSpacing( KDialog::spacingHint() );

    // table
    mPODTableModel = new PODTableModel( mTool, this );
    mPODTableView = new QTreeView( this );
    mPODTableView->setObjectName( "PODTable" );
    mPODTableView->setRootIsDecorated( false );
    mPODTableView->setAlternatingRowColors( true );
    mPODTableView->setItemsExpandable( false );
    mPODTableView->setUniformRowHeights( true );
    mPODTableView->setAllColumnsShowFocus( true );
//     mPODTableView->setEditTriggers( QAbstractItemView::AllEditTriggers );
    mPODTableView->setDragEnabled( true );
    mPODTableView->setSortingEnabled( false );
    mPODTableView->setModel( mPODTableModel );
    QHeaderView* header = mPODTableView->header();
    header->setResizeMode( QHeaderView::ResizeToContents );
    header->setStretchLastSection( false );
//     connect( mPODTableView, SIGNAL(doubleClicked( const QModelIndex& )),
//              SLOT(onDoubleClicked( const QModelIndex& )) );

    baseLayout->addWidget( mPODTableView, 10 );

    // settings
    QBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->setSpacing( KDialog::spacingHint() );
    settingsLayout->setMargin( 0 );

    mEndianSelection = new KComboBox( this );
    mEndianSelection->addItem( i18nc("@item:inlistbox","Little endian") ); // add first for index
    mEndianSelection->addItem( i18nc("@item:inlistbox","Big endian") );    // add second for index
    mEndianSelection->setCurrentIndex( mTool->byteOrder() );
    settingsLayout->addWidget( mEndianSelection, 0, Qt::AlignVCenter );
    connect( mEndianSelection, SIGNAL(activated( int )),
             mTool, SLOT(setByteOrder( int )));

    mUnsignedAsHexCheck = new QCheckBox( i18nc("@option:check","Unsigned as hexadecimal"), this );
    mUnsignedAsHexCheck->setChecked( mTool->isUnsignedAsHex() );
    settingsLayout->addWidget( mUnsignedAsHexCheck, 10, Qt::AlignVCenter );
    connect( mUnsignedAsHexCheck, SIGNAL(toggled( bool )),
             mTool, SLOT(setUnsignedAsHex( bool )) );

    baseLayout->addLayout( settingsLayout );
}


PODTableView::~PODTableView() {}
