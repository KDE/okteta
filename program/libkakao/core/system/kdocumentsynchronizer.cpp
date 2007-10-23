/***************************************************************************
                          kdocumentsynchronizer.cpp  -  description
                             -------------------
    begin                : Sun Sep 2 2007
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


#include "kdocumentsynchronizer.h"

// lib
#include "kistorable.h"
#include "kdocumentabstractfactory.h"
#include "kifileloader.h"
#include "kdocumentmanager.h"
// KDE
#include <kio/netaccess.h>
#include <KTemporaryFile>
#include <KLocale>
#include <KMessageBox>
#include <KFileDialog>


KDocumentSynchronizer::KDocumentSynchronizer( KDocumentManager *manager )
 : mManager( manager ), mWidget( 0 )
{
}

void KDocumentSynchronizer::setWidget( QWidget *widget )
{
    mWidget = widget;
}

void KDocumentSynchronizer::setDocumentFactory( KDocumentAbstractFactory *factory )
{
    mFactory = factory;
}

bool KDocumentSynchronizer::reload( KAbstractDocument *document )
{
Q_UNUSED(document)
/*    if( document->isModified() )
    {
        //sicher?
    }

    QString tmpFileName;
    const QString url = document->url().url();
    if( KIO::NetAccess::download(url,tmpFileName,mWidget) )
    {
        const bool success = document->load( tmpFileName );
        if( success )
        {
            emit reverted( document );
        }
        else
        {
            KMessageBox::error( mWidget, "Problem beim Laden" );
        }
        KIO::NetAccess::removeTempFile( tmpFileName );
    }
    else
        KMessageBox::error( mWidget, KIO::NetAccess::lastErrorString() );
*/
    return false;
}

bool store( KDE::If::Storable *storable, const KUrl &url, QWidget *widget )
{
    bool result = false;

        QString fileName;
        KTemporaryFile temporaryFile;

        if( url.isLocalFile() )
            fileName = url.path();
        else
        {
            temporaryFile.setAutoRemove( false );
            temporaryFile.open();
            fileName = temporaryFile.fileName();
        }

        if( storable->save( fileName ) )
        {
            if( !url.isLocalFile() )
            {
                result = KIO::NetAccess::upload( fileName, url, widget );
                if( !result )
                    KMessageBox::error( widget, KIO::NetAccess::lastErrorString() );
            }
        }
        else
            KMessageBox::error( widget, i18n("Problem when saving to local filesystem.") );


    return result;
}

bool KDocumentSynchronizer::save( KAbstractDocument *document )
{
    bool result = false;

    KDE::If::Storable *storable = document ? qobject_cast<KDE::If::Storable *>( document ) : 0;

    if( storable )
        result = store( storable, storable->url(), mWidget );

    return result;
}

bool KDocumentSynchronizer::saveAs( KAbstractDocument *document )
{
    bool storingDone = false;

    KDE::If::Storable *storable = document ? qobject_cast<KDE::If::Storable *>( document ) : 0;

    if( storable )
    {
        do
        {
            KUrl newUrl = KFileDialog::getSaveUrl( /*mWorkingUrl.url()*/QString(), QString(), mWidget );

            if( !newUrl.isEmpty() )
            {
                // new location?
                if( newUrl != storable->url() )
                {
                    const bool urlInUse = KIO::NetAccess::exists( newUrl, KIO::NetAccess::DestinationSide, mWidget );
                    if( urlInUse )
                    {
                    // TODO: care for case that file from url is already loaded by (only?) this program
//                         const bool otherFileLoaded = DocumentManager::documentByUrl( newUrl );
                        const QString title = i18n( "Save Document As..." );
                        const QString message = i18n( "There is already a file at \n\"%1\"\n. Overwrite?", newUrl.url() );
                        int answer = KMessageBox::warningYesNoCancel( mWidget, message, title, KStandardGuiItem::overwrite(), KStandardGuiItem::back() );
                        if( answer == KMessageBox::Cancel )
                            break;
                        if( answer == KMessageBox::No )
                            continue;
                    }
                }
                // store path
                storable->setUrl( newUrl.url() );
                store( storable, storable->url(), mWidget );
                storingDone = true;
#if 0
//     mWorkingUrl = Url.upUrl();
    OpenRecentAction->addUrl( Url );
#endif
            }
            else
                break;
       }
       while( !storingDone );
   }
   return storingDone;
}

bool KDocumentSynchronizer::saveElsewhere( KAbstractDocument *document )
{
Q_UNUSED(document)
/*
KUrl url = KFileDialog::getSaveUrl(m_doc->documentName(), "text/html", 0, i18n("Export File as HTML"));

if ( url.isEmpty() )
return;
*/
    return false;
}

bool KDocumentSynchronizer::saveCopy( KAbstractDocument *document )
{
Q_UNUSED(document)
    return false;
}

bool KDocumentSynchronizer::canClose( KAbstractDocument *document )
{
    bool canClose = true;

    if( document->hasLocalChanges() )
    {
        const QString title = i18n( "Close Document" );

        KDE::If::Storable *storable = document ? qobject_cast<KDE::If::Storable *>( document ) : 0;
        if( storable )
        {
            // "The document" should be defined by document->typename() or similar
            const QString message = i18n( "The document \"%1\" has been modified.\n"
                "Do you want to save your changes or discard them?", document->title() );
            const int answer = KMessageBox::warningYesNoCancel( mWidget, message, title,
                                                                KStandardGuiItem::save(), KStandardGuiItem::discard() );

            canClose = ( answer == KMessageBox::Yes ) ?
                           ( storable->url().isEmpty() ? saveAs( document ) :
                                                         save( document ) ) :
                       ( answer == KMessageBox::Cancel ) ? false :
                                                           true;
        }
        else
        {
            const QString message = i18n( "The document \"%1\" has been modified.\n"
                "Do you want to discard your changes?", document->title() );
            const int answer = KMessageBox::warningContinueCancel( mWidget, message, title,
                                                                   KStandardGuiItem::discard() );
            canClose = ( answer == KMessageBox::Continue );
        }
    }

    return canClose;
}

KDocumentSynchronizer::~KDocumentSynchronizer()
{
}


#include "kdocumentsynchronizer.moc"
