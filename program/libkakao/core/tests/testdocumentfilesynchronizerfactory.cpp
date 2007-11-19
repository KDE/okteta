/***************************************************************************
                          testdocumentfilesynchronizerfactory.h  -  description
                             -------------------
    begin                : Mon Nov 19 2007
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


#include "testdocumentfilesynchronizerfactory.h"

// lib
#include "testdocument.h"
#include "testdocumentfilesynchronizer.h"
// Qt
#include <QtCore/QLatin1String>

TestDocumentFileSynchronizerFactory::TestDocumentFileSynchronizerFactory() {}

// could be set to base class as value, is only one object per factory at runtime
// and settable in desktop file
QString TestDocumentFileSynchronizerFactory::supportedWorkType() const { return QLatin1String("TestDocument");}
QString TestDocumentFileSynchronizerFactory::supportedRemoteType() const { return QLatin1String("application/octet-stream");}

// TODO: these function seems to be always the same. Make macro or template function
// or, if there is only one place which calls this, move there
KAbstractDocument *TestDocumentFileSynchronizerFactory::loadNewDocument( const KUrl &originUrl ) const
{
    TestDocumentFileSynchronizer *synchronizer = new TestDocumentFileSynchronizer( originUrl );
    KAbstractDocument *document = synchronizer->document();
    if( !document )
        delete synchronizer;

    return document;
}

bool TestDocumentFileSynchronizerFactory::connectDocument( KAbstractDocument *document, const KUrl &originUrl,
                                  KAbstractDocumentSynchronizer::ConnectOption option ) const 
{
    TestDocumentFileSynchronizer *synchronizer = new TestDocumentFileSynchronizer( document, originUrl, option );
    // TODO: is synchronizer->document() really a good signal for success? see also below
    const bool success = ( synchronizer->document() != 0 );
    if( !success )
        delete synchronizer;

    return success;
}

bool TestDocumentFileSynchronizerFactory::exportDocument( KAbstractDocument *document, const KUrl &originUrl ) const
{
Q_UNUSED( document )
Q_UNUSED( originUrl )
    return false;
}

TestDocumentFileSynchronizerFactory::~TestDocumentFileSynchronizerFactory() {}
