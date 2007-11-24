/***************************************************************************
                          kabstractdocumentstreamencoder.h  -  description
                             -------------------
    begin                : Thu Nov 22 2007
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


#include "kabstractdocumentstreamencoder.h"

KAbstractDocumentStreamEncoder::KAbstractDocumentStreamEncoder( const QString &remoteTypeName,
                                                                    const QString &remoteMimeType )
: mRemoteTypeName( remoteTypeName ), mRemoteMimeType( remoteMimeType )
{}

QString KAbstractDocumentStreamEncoder::remoteTypeName() const { return mRemoteTypeName; }
QString KAbstractDocumentStreamEncoder::remoteMimeType() const { return mRemoteMimeType; }

KAbstractDocumentStreamEncoder::~KAbstractDocumentStreamEncoder() {}
