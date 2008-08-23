/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kdocumentsyncmanager.h"

// lib
#include "jobmanager.h"
#include "kdocumentmanager.h"
#include <abstractloadjob.h>
#include <abstractconnectjob.h>
#include <abstractsynctoremotejob.h>
#include <abstractsyncwithremotejob.h>
#include <abstractmodelsynchronizerfactory.h>
// documentaccess
#include <processdocumentconnectdialog.h>
// KDE
#include <KIO/NetAccess>
#include <KFileDialog>
#include <KMessageBox>
#include <KLocale>
// Qt
#include <QtGui/QApplication>


static const char AllFileNamesFilter[] = "*";

KDocumentSyncManager::KDocumentSyncManager( KDocumentManager *manager )
 : mManager( manager ), mWidget( 0 ), mSynchronizerFactory( 0 )
{}

void KDocumentSyncManager::setWidget( QWidget *widget )
{
    mWidget = widget;
}

bool KDocumentSyncManager::hasSynchronizerForLocal( const QString &workDocumentType ) const
{
    // TODO: need synchronizerfactory classes to query for this or a local datastructure
    return ( mSynchronizerFactory->supportedWorkType() == workDocumentType );
}

KUrl KDocumentSyncManager::urlOf( KAbstractDocument *document ) const
{
    AbstractModelSynchronizer* synchronizer = document->synchronizer();

    return synchronizer ? synchronizer->url() : KUrl();
}

void KDocumentSyncManager::setDocumentSynchronizerFactory( AbstractModelSynchronizerFactory* synchronizerFactory )
{
    mSynchronizerFactory = synchronizerFactory;
}

void KDocumentSyncManager::setDocumentLiveSynchronizerFactory( AbstractModelSynchronizerFactory* synchronizerFactory )
{
    mLiveSynchronizerFactory = synchronizerFactory;
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
    AbstractModelSynchronizer* synchronizer = mSynchronizerFactory->createSynchronizer();
    AbstractLoadJob *loadJob = synchronizer->startLoad( url );
    connect( loadJob, SIGNAL(documentLoaded( KAbstractDocument * )), SLOT(onDocumentLoaded( KAbstractDocument * )) );

    JobManager::executeJob( loadJob, mWidget );
}

void KDocumentSyncManager::onDocumentLoaded( KAbstractDocument *document )
{
    if( document )
        mManager->addDocument( document );
}

bool KDocumentSyncManager::setSynchronizer( KAbstractDocument *document )
{
    bool storingDone = false;

    AbstractModelSynchronizer* currentSynchronizer = document->synchronizer();
    // TODO: warn if there were updates in the second before saveAs was activated
//     if( currentSynchronizer )
//         currentSynchronizer->pauseSynchronization(); also unpause below
    const QString processTitle =
        i18nc( "@title:window Save %typename As...", "Save %1 As...", document->typeName() );
    do
    {
        KUrl newUrl = KFileDialog::getSaveUrl( /*mWorkingUrl.url()*/QString(), QString(), mWidget, processTitle );

        if( !newUrl.isEmpty() )
        {
            const bool isNewUrl = ( currentSynchronizer == 0 )
                                  || ( newUrl != currentSynchronizer->url() );

            if( isNewUrl )
            {
                const bool isUrlInUse = KIO::NetAccess::exists( newUrl, KIO::NetAccess::DestinationSide, mWidget );

                if( isUrlInUse )
                {
                    // TODO: care for case that file from url is already loaded by (only?) this program
//                     const bool otherFileLoaded = mManager->documentByUrl( newUrl );
                    // TODO: replace "file" with synchronizer->storageTypeName() or such
                    // TODO: offer "Synchronize" as alternative, if supported, see below
                    // ask synchronizer for capabilities, as some can only overwrite
                    const QString message =
                        i18nc( "@info",
                               "There is already a file at <nl/><filename>%1</filename>.<nl/>"
                               "Overwrite?", newUrl.url() );
                    int answer = KMessageBox::warningYesNoCancel( mWidget, message, processTitle, KStandardGuiItem::overwrite(), KStandardGuiItem::back() );
                    if( answer == KMessageBox::Cancel )
                        break;
                    if( answer == KMessageBox::No )
                        continue;
                }

                // switch url and synchronizer
                if( currentSynchronizer && true )//TODO: same remote mimetype
                {
                    //TODO: overwrite for now
                    AbstractSyncWithRemoteJob *syncJob = currentSynchronizer->startSyncWithRemote( newUrl,
                                                               AbstractModelSynchronizer::ReplaceRemote );
                    const bool syncSucceeded = JobManager::executeJob( syncJob, mWidget );
//                     currentSynchronizer->unpauseSynchronization(); also pause above
                    storingDone = syncSucceeded;
                }
                else
                {
                    //TODO: is overwrite for now, is this useful?
                    AbstractModelSynchronizer* synchronizer = mSynchronizerFactory->createSynchronizer();
                    AbstractConnectJob *connectJob = synchronizer->startConnect( document, newUrl,
                                                               AbstractModelSynchronizer::ReplaceRemote );
                    const bool connectSucceeded = JobManager::executeJob( connectJob, mWidget );

                    storingDone = connectSucceeded;
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
            {
                // TODO: what to do? synchTo? synchWith? synchFrom? Or does the synchronizer care for this?
                // By e.g. warning that we might be overwriting something?
                // synchTo might be the intention, after all the user wanted a new storage
                // 
                AbstractSyncToRemoteJob *syncJob = document->synchronizer()->startSyncToRemote();
                const bool syncFailed = JobManager::executeJob( syncJob, mWidget );

                storingDone = !syncFailed;
            }
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
        AbstractModelSynchronizer* synchronizer = document->synchronizer();
        const bool couldSynchronize = hasSynchronizerForLocal( document->mimeType() );

        const QString processTitle = i18nc( "@title:window Close %typename", "Close %1", document->typeName() );

        if( synchronizer || couldSynchronize )
        {
            const QString message = i18nc( "@info The %typename \"%title\" has been modified.",
                "The %1 <emphasis>%2</emphasis> has been modified.<nl/>"
                "Do you want to save your changes or discard them?", document->typeName(), document->title() );
            const int answer = KMessageBox::warningYesNoCancel( mWidget, message, processTitle,
                                                                KStandardGuiItem::save(), KStandardGuiItem::discard() );
            if( answer == KMessageBox::Yes )
            {
                if( synchronizer )
                {
                    AbstractSyncToRemoteJob *syncJob = synchronizer->startSyncToRemote();
                    const bool syncFailed = JobManager::executeJob( syncJob, mWidget );

                    canClose = !syncFailed;
                }
                else
                    canClose = setSynchronizer( document );
            }
            else
                canClose = ( answer == KMessageBox::No );
        }
        else
        {
            const QString message = i18nc( "@info The %typename \"%title\" has been modified.",
                "The %1 <emphasis>%2</emphasis> has been modified.<nl/>"
                "Do you want to discard your changes?", document->typeName(), document->title() );
            const int answer = KMessageBox::warningContinueCancel( mWidget, message, processTitle,
                                                                   KStandardGuiItem::discard() );
            canClose = ( answer == KMessageBox::Continue );
        }
    }

    return canClose;
}

void KDocumentSyncManager::startOffering( KAbstractDocument* document )
{
    AbstractModelSynchronizer* synchronizer = mLiveSynchronizerFactory->createSynchronizer();

    synchronizer->startOffering( document );
    //TODO: turn this into a job
}

void KDocumentSyncManager::finishOffering( KAbstractDocument* document )
{
    document->setLiveSynchronizer( 0 );
}

void KDocumentSyncManager::connectTo()
{
    ProcessDocumentConnectDialog* dialog = new ProcessDocumentConnectDialog( mWidget );

    if( dialog->exec() )
    {
        const KUrl url = dialog->getUrl();
        AbstractModelSynchronizer* synchronizer = mLiveSynchronizerFactory->createSynchronizer();
        AbstractLoadJob* loadJob = synchronizer->startLoad( url );
        connect( loadJob, SIGNAL(documentLoaded( KAbstractDocument * )), SLOT(onDocumentLoaded( KAbstractDocument * )) );
        JobManager::executeJob( loadJob, mWidget );

//         load( url );
        // store path
//         mWorkingUrl = url.upUrl();
//         emit urlUsed( url );
    }
    delete dialog;
}

KDocumentSyncManager::~KDocumentSyncManager()
{
    delete mSynchronizerFactory;
}
