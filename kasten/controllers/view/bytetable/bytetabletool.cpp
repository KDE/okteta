/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Okteta core
#include <character.h>
#include <charcodec.h>
#include <abstractbytearraymodel.h>
#include <changesdescribable.h>
// KDE
#include <KLocale>


namespace Kasten2
{

ByteTableTool::ByteTableTool()
 : mByteTableModel( new ByteTableModel(this) ),
   mByteArrayView( 0 ), mByteArrayModel( 0 )
{
    setObjectName( QLatin1String( "ByteTable" ) );
}

QString ByteTableTool::title() const { return i18nc("@title:window", "Value/Char Table"); }
ByteTableModel *ByteTableTool::byteTableModel() const { return mByteTableModel; }
bool ByteTableTool::hasWriteable() const
{
    return ( mByteArrayView && mByteArrayModel ) ? !mByteArrayView->isReadOnly() : false;
}


void ByteTableTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView )
    {
        mByteArrayView->disconnect( mByteTableModel );
        mByteArrayView->disconnect( this );
    }

    mByteArrayView = model ? qobject_cast<ByteArrayView*>( model ) : 0;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    const bool hasView = ( mByteArrayView && mByteArrayModel );
    if( hasView )
    {
        mByteTableModel->setCharCodec( mByteArrayView->charCodingName() );
        mByteTableModel->setUndefinedChar( mByteArrayView->undefinedChar() );
        connect( mByteArrayView,  SIGNAL(charCodecChanged(QString)),
                 mByteTableModel, SLOT(setCharCodec(QString)) );
        connect( mByteArrayView, SIGNAL(undefinedCharChanged(QChar)),
                 mByteArrayModel, SLOT(setUndefinedChar(QChar)) );

        connect( mByteArrayView, SIGNAL(readOnlyChanged(bool)),
                 SLOT(onReadOnlyChanged(bool)) );
    }

    const bool isWriteable = ( hasView && !mByteArrayView->isReadOnly() );

    emit hasWriteableChanged( isWriteable );
}

void ByteTableTool::insert( unsigned char byte, int count )
{
    const QByteArray data( count, byte );

    Okteta::ChangesDescribable *changesDescribable =
        qobject_cast<Okteta::ChangesDescribable*>( mByteArrayModel );

    if( changesDescribable )
    {
        // TODO: how to note the byte? charcoding might change...
        const QString changeDescription =
            i18np( "Inserted 1 Byte","Inserted %1 Bytes", count );

        changesDescribable->openGroupedChange( changeDescription );
    }

    mByteArrayView->insert( data );

    if( changesDescribable )
        changesDescribable->closeGroupedChange();
// void ByteTableController::fill( const QByteArray &Data )
// {
//     if( HexEdit && ByteArray )
//         ByteArray->insert( HexEdit->cursorPosition(), Data );
// }
    mByteArrayView->setFocus();
}

void ByteTableTool::onReadOnlyChanged( bool isReadOnly )
{
    const bool isWriteable = !isReadOnly;

    emit hasWriteableChanged( isWriteable );
}

ByteTableTool::~ByteTableTool() {}

}
