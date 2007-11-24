/***************************************************************************
                          kdocumentsyncmanager.cpp  -  description
                             -------------------
    begin                : Wed Nov 14 2007
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


#include "kdocumentsyncmanager.h"

// lib
#include <kabstractdocumentsynchronizerfactory.h>
#include "kdocumentmanager.h"
// KDE
#include <KIO/NetAccess>
#include <KFileDialog>
#include <KMessageBox>
#include <KLocale>


static const char AllFileNamesFilter[] = "*";

KDocumentSyncManager::KDocumentSyncManager( KDocumentManager *manager )
 : mManager( manager ), mWidget( 0 ), mSynchronizerFactory( 0 )
{}

void KDocumentSyncManager::setWidget( QWidget *widget )
{
    mWidget = widget;
}

bool KDocumentSyncManager::hasSynchronizerForLocal( const QString &workDocumentType )
{
    // TODO: need synchronizerfactory classes to query for this or a local datastructure
    return ( mSynchronizerFactory->supportedWorkType() == workDocumentType );
}

void KDocumentSyncManager::setDocumentSynchronizerFactory( KAbstractDocumentSynchronizerFactory *synchronizerFactory )
{
    mSynchronizerFactory = synchronizerFactory;
}

void KDocumentSyncManager::load()
{
    KUrl url = KFileDialog::getOpenUrl( QString()/*mWorkingUrl.url()*/, AllFileNamesFilter, mWidget );

    if( !url.isEmpty() )
    {
        load( url );
        // store path
//         mWorkingUrl = url.upUrl();
        emit urlUsed( url );
    }
}

void KDocumentSyncManager::load( const KUrl &url )
{
    KAbstractDocument *document = mSynchronizerFactory->loadNewDocument( url );

    if( document )
        mManager->addDocument( document );
}

bool KDocumentSyncManager::setSynchronizer( KAbstractDocument *document )
{
    bool storingDone = false;

    KAbstractDocumentSynchronizer *oldSynchronizer = document->synchronizer();
    // TODO: warn if there were updates in the second before saveAs was activated
//     if( oldSynchronizer )
//         oldSynchronizer->pauseSynchronization(); also unpause below
    do
    {
        KUrl newUrl = KFileDialog::getSaveUrl( /*mWorkingUrl.url()*/QString(), QString(), mWidget );

        if( !newUrl.isEmpty() )
        {
            const bool isNewUrl = ( oldSynchronizer == 0 )
                                  || ( newUrl != oldSynchronizer->url() );

            if( isNewUrl )
            {
                const bool isUrlInUse = KIO::NetAccess::exists( newUrl, KIO::NetAccess::DestinationSide, mWidget );

                if( isUrlInUse )
                {
                    // TODO: care for case that file from url is already loaded by (only?) this program
//                     const bool otherFileLoaded = mManager->documentByUrl( newUrl );
                    const QString title = i18nc( "Save %typename As...", "Save %1 As...", document->typeName() );
                    // TODO: replace "file" with synchronizer->storageTypeName() or such
                    // TODO: offer "Synchronize" as alternative, if supported, see below
                    const QString message = i18n( "There is already a file at \n\"%1\".\n Overwrite?", newUrl.url() );
                    int answer = KMessageBox::warningYesNoCancel( mWidget, message, title, KStandardGuiItem::overwrite(), KStandardGuiItem::back() );
                    if( answer == KMessageBox::Cancel )
                        break;
                    if( answer == KMessageBox::No )
                        continue;
                }

                // switch url and synchronizer
                if( oldSynchronizer && true )//TODO: same remote mimetype
                {
                    //TODO: ioverwrite for now
                    oldSynchronizer->syncWithRemote( newUrl, KAbstractDocumentSynchronizer::ReplaceRemote );
//                     oldSynchronizer->unpauseSynchronization(); also pause above
                    storingDone = true;
                }
                else
                {
                    //TODO: overwrite for now
                    storingDone =
                        mSynchronizerFactory->connectDocument( document, newUrl,
                                                               KAbstractDocumentSynchronizer::ReplaceRemote );
                }

                if( storingDone )
                    emit urlUsed( newUrl );
#if 0
//     mWorkingUrl = Url.upUrl();
    OpenRecentAction->addUrl( Url );
#endif
            }
            // same url
            else
                // TODO: what to do? synchTo? synchWith? synchFrom? Or does the synchronizer care for this?
                // By e.g. warning that we might be overwriting something?
                // synchTo might be the intention, after all the user wanted a new storage
                // 
                document->synchronizer()->syncToRemote();
        }
        else
            break;
   }
   while( !storingDone );

   return storingDone;
}

bool KDocumentSyncManager::canClose( KAbstractDocument *document )
{
    bool canClose = true;

    if( document->hasLocalChanges() )
    {
        KAbstractDocumentSynchronizer *synchronizer = document->synchronizer();
        const bool couldSynchronize = hasSynchronizerForLocal( document->mimeType() );

        const QString title = i18nc( "Close %typename", "Close %1", document->typeName() );

        if( synchronizer || couldSynchronize )
        {
            const QString message = i18nc( "The %typename \"%title\" has been modified.",
                "The %1 \"%2\" has been modified.\n"
                "Do you want to save your changes or discard them?", document->typeName(), document->title() );
            const int answer = KMessageBox::warningYesNoCancel( mWidget, message, title,
                                                                KStandardGuiItem::save(), KStandardGuiItem::discard() );
            if( answer == KMessageBox::Yes )
            {
                if( synchronizer )
                    canClose = synchronizer->syncToRemote();
                else
                    canClose = setSynchronizer( document );
            }
            else
                canClose = ( answer == KMessageBox::No );
        }
        else
        {
            const QString message = i18nc( "The %typename \"%title\" has been modified.",
                "The %1 \"%2\" has been modified.\n"
                "Do you want to discard your changes?", document->typeName(), document->title() );
            const int answer = KMessageBox::warningContinueCancel( mWidget, message, title,
                                                                   KStandardGuiItem::discard() );
            canClose = ( answer == KMessageBox::Continue );
        }
    }

    return canClose;
}

KDocumentSyncManager::~KDocumentSyncManager()
{
    delete mSynchronizerFactory;
}
