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
#include <kbytearraymodel.h>


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
        KHECore::KByteArrayModel *byteArray = byteArrayDocument->content();
        const char *data = byteArray->data();
        const int size = byteArray->size();

        success = encodeDataToStream( device, data, size );
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
        KHECore::KByteArrayModel *byteArray = byteArrayDocument->content();
        const KHE::KSection section = byteArraySelection->isValid() ? byteArraySelection->section() : KHE::KSection( 0, byteArray->size() );
        const char *data = byteArray->data() + section.start();
        const int size = section.width();

        success = encodeDataToStream( device, data, size );
    }
    return success;
}

KAbstractByteArrayStreamEncoder::~KAbstractByteArrayStreamEncoder() {}
