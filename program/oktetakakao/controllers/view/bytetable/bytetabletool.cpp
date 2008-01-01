/***************************************************************************
                          bytetabletool.cpp  -  description
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


#include "bytetabletool.h"

// controller
#include "bytetablemodel.h"
//
#include <kbytearraydocument.h>
// Kakao
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
#include <kbytearraymodel.h>


ByteTableTool::ByteTableTool()
 : mByteTableModel( new ByteTableModel(this) ),
   mByteArrayView( 0 ), mByteArrayModel( 0 )
{
}

ByteTableModel *ByteTableTool::byteTableModel() const { return mByteTableModel; }
bool ByteTableTool::hasByteArrayView() const { return ( mByteArrayView != 0 ); }


void ByteTableTool::setView( KAbstractView *view )
{
    disconnect( mByteArrayView, 0, mByteTableModel, 0 );

    mByteArrayView = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    KByteArrayDocument *document = view ? static_cast<KByteArrayDocument*>( view->document() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayView )
    {
        mByteTableModel->setCharCodec( mByteArrayView->encodingName() );
        connect( mByteArrayView,  SIGNAL(charCodecChanged( const QString & )),
                 mByteTableModel, SLOT(setCharCodec( const QString &)) );
    }
    emit byteArrayViewChanged( mByteArrayView != 0 );
}

void ByteTableTool::insert( unsigned char byte, int count )
{
    const QByteArray data( count, byte );

    mByteArrayView->insert( data );
// void ByteTableController::fill( const QByteArray &Data )
// {
//     if( HexEdit && ByteArray )
//         ByteArray->insert( HexEdit->cursorPosition(), Data );
// }
}


ByteTableTool::~ByteTableTool() {}
