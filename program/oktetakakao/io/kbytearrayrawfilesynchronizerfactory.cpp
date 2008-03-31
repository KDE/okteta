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

#include "kbytearrayrawfilesynchronizerfactory.h"

// lib
#include "kbytearraydocument.h"
#include "kbytearrayrawfilesynchronizer.h"
// Qt
#include <QtCore/QLatin1String>
// kde
#include <kurl.h>

KByteArrayRawFileSynchronizerFactory::KByteArrayRawFileSynchronizerFactory() {}

// could be set to base class as value, is only one object per factory at runtime
// and settable in desktop file
QString KByteArrayRawFileSynchronizerFactory::supportedWorkType() const { return QLatin1String("KByteArrayDocument");}
QString KByteArrayRawFileSynchronizerFactory::supportedRemoteType() const { return QLatin1String("application/octet-stream");}

// TODO: these function seems to be always the same. Make macro or template function
// or, if there is only one place which calls this, move there
KAbstractDocument *KByteArrayRawFileSynchronizerFactory::loadNewDocument( const KUrl &originUrl ) const
{
    KByteArrayRawFileSynchronizer *synchronizer = new KByteArrayRawFileSynchronizer( originUrl );
    KAbstractDocument *document = synchronizer->document();
    if( !document )
        delete synchronizer;

    return document;
}

bool KByteArrayRawFileSynchronizerFactory::connectDocument( KAbstractDocument *document, const KUrl &originUrl,
                                  KAbstractDocumentSynchronizer::ConnectOption option ) const 
{
    KByteArrayRawFileSynchronizer *synchronizer = new KByteArrayRawFileSynchronizer( document, originUrl, option );
    // TODO: is synchronizer->document() really a good signal for success? see also below
    const bool success = ( synchronizer->document() != 0 );
    if( !success )
        delete synchronizer;

    return success;
}

bool KByteArrayRawFileSynchronizerFactory::exportDocument( KAbstractDocument *document, const KUrl &originUrl ) const
{
Q_UNUSED( document )
Q_UNUSED( originUrl )
    return false;
}

KByteArrayRawFileSynchronizerFactory::~KByteArrayRawFileSynchronizerFactory() {}
