/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <KDialog>
#include <KLocale>
#include <KStandardGuiItem>
#include <KGlobalSettings>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QSpinBox>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>


ByteTableView::ByteTableView( ByteTableTool *tool, QWidget *parent )
 : QWidget( parent ), mTool( tool )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );
    baseLayout->setSpacing( KDialog::spacingHint() );

    mByteTableView = new QTreeView( this );
    mByteTableView->setObjectName( "ByteTable" );
    mByteTableView->setRootIsDecorated( false );
    mByteTableView->setItemsExpandable( false );
    mByteTableView->setUniformRowHeights( true );
    mByteTableView->setAllColumnsShowFocus( true );
    mByteTableView->setSortingEnabled( false );
    mByteTableView->setFont( KGlobalSettings::fixedFont() );
    mByteTableView->header()->setFont( font() );
    mByteTableView->setModel( mTool->byteTableModel() );
    for( int c = 0; c<ByteTableModel::NoOfIds; ++c )
        mByteTableView->resizeColumnToContents( c );
    connect( mByteTableView, SIGNAL(doubleClicked( const QModelIndex& )),
             SLOT(onDoubleClicked( const QModelIndex& )) );

    baseLayout->addWidget( mByteTableView, 10 );

    QHBoxLayout *insertLayout = new QHBoxLayout();

    QLabel *label = new QLabel( i18nc("@label:spinbox","Number of bytes:"), this );
    label->setFixedWidth( label->sizeHint().width() );
    insertLayout->addWidget( label );

    mInsertCountSpinBox = new QSpinBox( this );
//     mInsertCountSpinBox->setMinimumWidth( mInsertCountSpinBox->fontMetrics().maxWidth()*4 );
    mInsertCountSpinBox->setRange( 1, INT_MAX );
    mInsertCountSpinBox->setValue( 1 );
    insertLayout->addWidget( mInsertCountSpinBox );
    mInsertButton = new KPushButton( KStandardGuiItem::insert(), this );
    mInsertButton->setEnabled( mTool->hasByteArrayView() );
    connect( mTool, SIGNAL(byteArrayViewChanged(bool)), mInsertButton, SLOT( setEnabled(bool )) );
    connect( mInsertButton, SIGNAL(clicked(bool)), SLOT(onInsertClicked()) ); 
    insertLayout->addWidget( mInsertButton );

    baseLayout->addLayout( insertLayout );
}


void ByteTableView::onDoubleClicked( const QModelIndex &index )
{
    if( !mTool->hasByteArrayView() )
        return;

    const unsigned char byte = index.row();
    mTool->insert( byte, mInsertCountSpinBox->value() );
}

void ByteTableView::onInsertClicked()
{
    const unsigned char byte = mByteTableView->currentIndex().row();
    mTool->insert( byte, mInsertCountSpinBox->value() );
}

ByteTableView::~ByteTableView() {}
