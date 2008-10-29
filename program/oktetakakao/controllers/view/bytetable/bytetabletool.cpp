/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <kbytearraydisplay.h>
#include <kbytearraydocument.h>
// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
#include <kabstractbytearraymodel.h>
#include <changesdescribable.h>
// KDE
#include <KLocale>


ByteTableTool::ByteTableTool()
 : mByteTableModel( new ByteTableModel(this) ),
   mByteArrayDisplay( 0 ), mByteArrayModel( 0 )
{
    setObjectName( "ByteTable" );
}

QString ByteTableTool::title() const { return i18nc("@title:window", "Byte Table"); }
ByteTableModel *ByteTableTool::byteTableModel() const { return mByteTableModel; }
bool ByteTableTool::hasByteArrayView() const { return ( mByteArrayDisplay != 0 ); }


void ByteTableTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayDisplay ) mByteArrayDisplay->disconnect( mByteTableModel );

    mByteArrayDisplay = model ? qobject_cast<KByteArrayDisplay*>( model ) : 0;

    KByteArrayDocument* document =
        mByteArrayDisplay ? qobject_cast<KByteArrayDocument*>( mByteArrayDisplay->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    const bool hasView = ( mByteArrayDisplay && mByteArrayModel );
    if( hasView )
    {
        mByteTableModel->setCharCodec( mByteArrayDisplay->charCodingName() );
        connect( mByteArrayDisplay,  SIGNAL(charCodecChanged( const QString& )),
                 mByteTableModel, SLOT(setCharCodec( const QString& )) );
    }
    emit byteArrayViewChanged( hasView );
}

void ByteTableTool::insert( unsigned char byte, int count )
{
    const QByteArray data( count, byte );

    KHECore::ChangesDescribable *changesDescribable =
        qobject_cast<KHECore::ChangesDescribable*>( mByteArrayModel );

    if( changesDescribable )
        changesDescribable->openGroupedChange( i18n("Byte inserted.") );
    mByteArrayDisplay->insert( data );
    if( changesDescribable )
        changesDescribable->closeGroupedChange();
// void ByteTableController::fill( const QByteArray &Data )
// {
//     if( HexEdit && ByteArray )
//         ByteArray->insert( HexEdit->cursorPosition(), Data );
// }
}


ByteTableTool::~ByteTableTool() {}
