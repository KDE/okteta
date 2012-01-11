/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraydocumentfactory.h"

// lib
#include "bytearraydocument.h"
// Okteta core
#include <piecetablebytearraymodel.h>
// Kasten core
#include <person.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QMimeData>


namespace Kasten2
{

static int newByteArrayDocumentCounter = 0;
static const char OctetStreamFormatName[] = "application/octet-stream";

bool ByteArrayDocumentFactory::canCreateFromData( const QMimeData* mimeData )
{
    Q_UNUSED( mimeData );

    // we currently take everything, see createFromData()
    return true;
}

AbstractDocument* ByteArrayDocumentFactory::create()
{
    ByteArrayDocument* document = new ByteArrayDocument( i18nc("The byte array was new created.","New created.") );

    ++newByteArrayDocumentCounter;

    // TODO: use document->typeName() ?
    document->setTitle(
        i18ncp("numbered title for a created document without a filename",
               "[New Byte Array]","[New Byte Array %1]",newByteArrayDocumentCounter) );

    document->setOwner( Person::createEgo() );

    return document;
}

AbstractDocument* ByteArrayDocumentFactory::createFromData( const QMimeData* mimeData, bool setModified )
{
    if( ! mimeData || mimeData->formats().isEmpty() )
        return create();

    // SYNC: with abstractbytearrayview_p.cpp
    // if there is a octet stream, use it, otherwise take the dump of the format
    // with the highest priority
    // TODO: this may not be, what is expected, think about it, if we just
    // take byte array descriptions, like encodings in chars or values
    // would need the movement of the encoders into the core library
    const QString octetStreamFormatName = QString::fromLatin1( OctetStreamFormatName );
    const QString dataFormatName = ( mimeData->hasFormat(octetStreamFormatName) ) ?
        octetStreamFormatName :
        mimeData->formats()[0];

    const QByteArray data = mimeData->data( dataFormatName );

    Okteta::PieceTableByteArrayModel* byteArray =
        new Okteta::PieceTableByteArrayModel( data );
    byteArray->setModified( setModified );

    // TODO: pass name of generator
    ByteArrayDocument* document = new ByteArrayDocument( byteArray, i18nc("origin of the byte array", "Created from data.") );

    ++newByteArrayDocumentCounter;

    // TODO: use document->typeName() ?
    document->setTitle(
        i18ncp("numbered title for a created document without a filename",
               "[New Byte Array]","[New Byte Array %1]",newByteArrayDocumentCounter) );

    document->setOwner( Person::createEgo() );

    return document;
}

}
