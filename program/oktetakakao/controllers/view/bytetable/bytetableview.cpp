/***************************************************************************
                          bytetableview.cpp  -  description
                             -------------------
    begin                : Mon Dec 31 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


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

    QLabel *label = new QLabel( i18n("Number of bytes:"), this );
    label->setFixedWidth( label->sizeHint().width() );
    insertLayout->addWidget( label );

    mInsertCountSpinBox = new QSpinBox( this );
//     mInsertCountSpinBox->setMinimumWidth( mInsertCountSpinBox->fontMetrics().maxWidth()*4 );
//     mInsertCountSpinBox->setRange( 1, INT_MAX );
    mInsertCountSpinBox->setValue( 1 );
    insertLayout->addWidget( mInsertCountSpinBox );
    mInsertButton = new KPushButton( KStandardGuiItem::insert(), this );
    mInsertButton->setEnabled( mTool->byteTableModel() != 0 );
    connect( mTool, SIGNAL(modelChanged(bool)), mInsertButton, SLOT( setEnabled(bool )) );
    connect( mInsertButton, SIGNAL(clicked(bool)), SLOT(onInsertClicked()) ); 
    insertLayout->addWidget( mInsertButton );

    baseLayout->addLayout( insertLayout );
}


void ByteTableView::onDoubleClicked( const QModelIndex &index )
{
    if( mTool->byteTableModel() == 0 )
        return;

    const unsigned char byte = index.row();
    mTool->insert( byte, mInsertCountSpinBox->value() );
}

void ByteTableView::onInsertClicked()
{
    mTool->insert( mByteTableView->currentIndex().row(), mInsertCountSpinBox->value() );
}

ByteTableView::~ByteTableView() {}
