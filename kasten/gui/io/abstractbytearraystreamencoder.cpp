/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#include "abstractbytearraystreamencoder.h"

// lib
#include <bytearrayview.h>
#include <bytearraydocument.h>
#include <kbytearrayselection.h>
// Okteta core
#include <abstractbytearraymodel.h>
// Qt
#include <QtCore/QBuffer>


namespace Kasten
{

static const int MaxPreviewSize = 100;


AbstractByteArrayStreamEncoder::AbstractByteArrayStreamEncoder( const QString &remoteTypeName,
                                                                  const QString &remoteMimeType )
 : AbstractModelStreamEncoder( remoteTypeName, remoteMimeType )
{}

QString AbstractByteArrayStreamEncoder::modelTypeName( AbstractModel* model, const AbstractModelSelection* selection ) const
{
Q_UNUSED( selection )

    const ByteArrayDocument* byteArrayDocument = model->findBaseModel<const ByteArrayDocument*>();

    return ( byteArrayDocument == 0 ) ? QString() : byteArrayDocument->typeName();
}


bool AbstractByteArrayStreamEncoder::encodeToStream( QIODevice* device,
                                                     AbstractModel* model, const AbstractModelSelection* selection )
{
    const ByteArrayView* byteArrayDisplay = qobject_cast<const ByteArrayView*>( model );

    const ByteArrayDocument* byteArrayDocument =
        byteArrayDisplay ? qobject_cast<const ByteArrayDocument*>( byteArrayDisplay->baseModel() ) : 0;
    if( byteArrayDocument == 0 )
        return false;

    const Okteta::AbstractByteArrayModel* byteArray = byteArrayDocument->content();

    const KByteArraySelection* byteArraySelection =
        selection ? static_cast<const KByteArraySelection*>( selection ) : 0;

    const Okteta::AddressRange range = byteArraySelection && byteArraySelection->isValid() ?
        byteArraySelection->range() :
        Okteta::AddressRange::fromWidth( 0, byteArray->size() );

    const bool success = encodeDataToStream( device, byteArrayDisplay, byteArray, range );

    return success;
}

QString AbstractByteArrayStreamEncoder::previewData( AbstractModel* model, const AbstractModelSelection* selection )
{
    const ByteArrayView* byteArrayDisplay = qobject_cast<const ByteArrayView*>( model );

    const ByteArrayDocument* byteArrayDocument =
        byteArrayDisplay ? qobject_cast<const ByteArrayDocument*>( byteArrayDisplay->baseModel() ) : 0;
    if( byteArrayDocument == 0 )
        return QString();

    const Okteta::AbstractByteArrayModel* byteArray = byteArrayDocument->content();

    const KByteArraySelection* byteArraySelection =
        selection ? static_cast<const KByteArraySelection*>( selection ) : 0;

    Okteta::AddressRange range = byteArraySelection && byteArraySelection->isValid() ?
        byteArraySelection->range() :
        Okteta::AddressRange::fromWidth( 0, byteArray->size() );
    range.restrictEndByWidth( MaxPreviewSize );

    QByteArray data;
    QBuffer dataBuffer( &data );
    dataBuffer.open( QIODevice::WriteOnly );

    const bool success = encodeDataToStream( &dataBuffer, byteArrayDisplay, byteArray, range );
    dataBuffer.close();

    return success ? QString(data) : QString();
}

AbstractByteArrayStreamEncoder::~AbstractByteArrayStreamEncoder() {}

}
