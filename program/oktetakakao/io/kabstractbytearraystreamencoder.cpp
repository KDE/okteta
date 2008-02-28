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
#include "kbytearraydocument.h"
#include "kbytearrayselection.h"
// Okteta core
#include <kabstractbytearraymodel.h>


KAbstractByteArrayStreamEncoder::KAbstractByteArrayStreamEncoder( const QString &remoteTypeName,
                                                                  const QString &remoteMimeType )
 : KAbstractDocumentStreamEncoder( remoteTypeName, remoteMimeType )
{}

bool KAbstractByteArrayStreamEncoder::encodeToStream( QIODevice *device,
                                                      const KAbstractDocument *document )
{
    bool success = false;

    const KByteArrayDocument *byteArrayDocument = qobject_cast<const KByteArrayDocument*>( document );
    if( byteArrayDocument )
    {
        KHECore::KAbstractByteArrayModel *byteArray = byteArrayDocument->content();
        const int size = byteArray->size();

        success = encodeDataToStream( device, byteArray, KHE::KSection::fromWidth(0,size) );
    }

    return success;
}

bool KAbstractByteArrayStreamEncoder::encodeToStream( QIODevice *device,
                                                      const KAbstractDocumentSelection *selection )
{
    bool success = false;
    const KByteArraySelection *byteArraySelection = qobject_cast<const KByteArraySelection*>( selection );
    const KByteArrayDocument *byteArrayDocument = qobject_cast<const KByteArrayDocument*>( selection->document() );
    if( byteArrayDocument && byteArraySelection )
    {
        KHECore::KAbstractByteArrayModel *byteArray = byteArrayDocument->content();
        const KHE::KSection section = byteArraySelection->isValid() ? byteArraySelection->section() :
                                      KHE::KSection::fromWidth( 0, byteArray->size() );

        success = encodeDataToStream( device, byteArray, section );
    }
    return success;
}

KAbstractByteArrayStreamEncoder::~KAbstractByteArrayStreamEncoder() {}
