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

#include "bytetabletool.h"

// controller
#include "bytetablemodel.h"
// lib
#include <kbytearraydocument.h>
// Kakao gui
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
    if( mByteArrayView ) mByteArrayView->disconnect( mByteTableModel );

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

#include "bytetabletool.moc"
