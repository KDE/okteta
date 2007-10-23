/***************************************************************************
                          kdocumentmanager.cpp  -  description
                             -------------------
    begin                : Fri Jun 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kdocumentcreator.h"

// lib
#include "kdocumentabstractfactory.h"
#include "kifileloader.h"
#include "kistorable.h"
#include "kdocumentmanager.h"
// KDE
#include <kio/netaccess.h>
#include <KTemporaryFile>
#include <KMessageBox>


KDocumentCreator::KDocumentCreator( KDocumentManager *manager )
 : mManager( manager ) {}

void KDocumentCreator::setWidget( QWidget *widget )
{
    mWidget = widget;
}

void KDocumentCreator::setDocumentFactory( KDocumentAbstractFactory *factory )
{
    mFactory = factory;
}


void KDocumentCreator::createNew()
{
   KAbstractDocument *document = mFactory->create();
   mManager->addDocument( document );
}


void KDocumentCreator::open( const KUrl &url )
{
    KDE::If::FileLoader *fileLoader = qobject_cast<KDE::If::FileLoader *>( mFactory );
    if( !fileLoader )
        return;

    QString tmpFileName;
    if( KIO::NetAccess::download(url.url(),tmpFileName,mWidget) )
    {
        KAbstractDocument *document = fileLoader->load( tmpFileName, url.url() );
        if( document )
        {
            mManager->addDocument( document );
        }
        else
        {
            KMessageBox::error( mWidget, "Problem beim Laden" );
        }
        KIO::NetAccess::removeTempFile( tmpFileName );
    }
    else
        KMessageBox::error( mWidget, KIO::NetAccess::lastErrorString() );
}


KDocumentCreator::~KDocumentCreator()
{
//     delete mFactory;
}


#include "kdocumentcreator.moc"
