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

#include "kabstractbytearraystreamencoder.h"

// lib
#include <kbytearraydisplay.h>
#include <kbytearraydocument.h>
#include <kbytearrayselection.h>
// Okteta core
#include <abstractbytearraymodel.h>
// Qt
#include <QtCore/QBuffer>


static const int MaxPreviewSize = 100;


KAbstractByteArrayStreamEncoder::KAbstractByteArrayStreamEncoder( const QString &remoteTypeName,
                                                                  const QString &remoteMimeType )
 : AbstractModelStreamEncoder( remoteTypeName, remoteMimeType )
{}

QString KAbstractByteArrayStreamEncoder::modelTypeName( AbstractModel *model, const AbstractModelSelection *selection ) const
{
Q_UNUSED( selection )

    const KByteArrayDocument* byteArrayDocument = model->findBaseModel<const KByteArrayDocument*>();

    return ( byteArrayDocument == 0 ) ? QString() : byteArrayDocument->typeName();
}


bool KAbstractByteArrayStreamEncoder::encodeToStream( QIODevice *device,
                                                      AbstractModel *model, const AbstractModelSelection *selection )
{
    const KByteArrayDisplay* byteArrayDisplay = qobject_cast<const KByteArrayDisplay*>( model );

    const KByteArrayDocument* byteArrayDocument =
        byteArrayDisplay ? qobject_cast<const KByteArrayDocument*>( byteArrayDisplay->baseModel() ) : 0;
    if( byteArrayDocument == 0 )
        return false;

    const KHECore::AbstractByteArrayModel* byteArray = byteArrayDocument->content();

    const KByteArraySelection* byteArraySelection =
        selection ? static_cast<const KByteArraySelection*>( selection ) : 0;

    const KHE::Section section = byteArraySelection && byteArraySelection->isValid() ? byteArraySelection->section() :
                                   KHE::Section::fromWidth( 0, byteArray->size() );

    const bool success = encodeDataToStream( device, byteArrayDisplay, byteArray, section );

    return success;
}

QString KAbstractByteArrayStreamEncoder::previewData( AbstractModel* model, const AbstractModelSelection* selection )
{
    const KByteArrayDisplay* byteArrayDisplay = qobject_cast<const KByteArrayDisplay*>( model );

    const KByteArrayDocument* byteArrayDocument =
        byteArrayDisplay ? qobject_cast<const KByteArrayDocument*>( byteArrayDisplay->baseModel() ) : 0;
    if( byteArrayDocument == 0 )
        return QString();

    const KHECore::AbstractByteArrayModel* byteArray = byteArrayDocument->content();

    const KByteArraySelection* byteArraySelection =
        selection ? static_cast<const KByteArraySelection*>( selection ) : 0;

    KHE::Section section = byteArraySelection && byteArraySelection->isValid() ?
        byteArraySelection->section() :
        KHE::Section::fromWidth( 0, byteArray->size() );
    section.restrictEndByWidth( MaxPreviewSize );

    QByteArray data;
    QBuffer dataBuffer( &data );
    dataBuffer.open( QIODevice::WriteOnly );

    const bool success = encodeDataToStream( &dataBuffer, byteArrayDisplay, byteArray, section );
    dataBuffer.close();

    return success ? QString(data) : QString();
}

KAbstractByteArrayStreamEncoder::~KAbstractByteArrayStreamEncoder() {}
