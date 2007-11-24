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


#ifndef KABSTRACTBYTEARRAYSTREAMENCODER_H
#define KABSTRACTBYTEARRAYSTREAMENCODER_H

// kakao
#include <kabstractdocumentstreamencoder.h>

class KAbstractByteArrayStreamEncoder : public KAbstractDocumentStreamEncoder
{
    Q_OBJECT

  public:
    KAbstractByteArrayStreamEncoder( const QString &remoteTypeName, const QString &remoteMimeType );
    virtual ~KAbstractByteArrayStreamEncoder();

  public: // KAbstractDocumentStreamEncoder API
    virtual bool encodeToStream( QIODevice *device, const KAbstractDocument *document );
    virtual bool encodeToStream( QIODevice *device, const KAbstractDocumentSelection *selection );

  protected: // API to be implemented
    virtual bool encodeDataToStream( QIODevice *device, const char *data, int size ) = 0;
};

#endif
