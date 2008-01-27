/***************************************************************************
                          kabstractbytearraystreamencoder.h  -  description
                             -------------------
    begin                : Sat Nov 24 2007
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
