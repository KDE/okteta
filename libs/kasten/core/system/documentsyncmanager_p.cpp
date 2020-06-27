/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentsyncmanager_p.hpp"

// lib
#include "abstractoverwritedialog.hpp"
#include "abstractsavediscarddialog.hpp"
#include "jobmanager.hpp"
#include "documentmanager.hpp"
#include <abstractloadjob.hpp>
#include <abstractconnectjob.hpp>
#include <abstractsynctoremotejob.hpp>
#include <abstractsyncwithremotejob.hpp>
#include <abstractsyncfromremotejob.hpp>
#include <abstractmodelsynchronizerfactory.hpp>
#include <abstractdocument.hpp>
// KF
#include <KIO/StatJob>
#include <KJobWidgets>
#include <KLocalizedString>
// Qt
#include <QUrl>
#include <QFileDialog>
#include <QMimeDatabase>

namespace Kasten {

DocumentSyncManagerPrivate::DocumentSyncManagerPrivate(DocumentSyncManager* q, DocumentManager* manager)
    : q_ptr(q)
    , mManager(manager)
{}

DocumentSyncManagerPrivate::~DocumentSyncManagerPrivate()
{
    delete mSynchronizerFactory;
}

void DocumentSyncManagerPrivate::setSaveDiscardDialog(AbstractSaveDiscardDialog* saveDiscardDialog)
{
    mSaveDiscardDialog = saveDiscardDialog;
}

void DocumentSyncManagerPrivate::setOverwriteDialog(AbstractOverwriteDialog* overwriteDialog)
{
    mOverwriteDialog = overwriteDialog;
}

// TODO: make a difference between stream/storage formats and work/live formats?
QStringList DocumentSyncManagerPrivate::supportedRemoteTypes() const
{
    return {mSynchronizerFactory->supportedRemoteType()};
}

bool DocumentSyncManagerPrivate::hasSynchronizerForLocal(const QString& workDocumentType) const
{
    // TODO: need synchronizerfactory classes to query for this or a local datastructure
    return (mSynchronizerFactory->supportedWorkType() == workDocumentType);
}

QUrl DocumentSyncManagerPrivate::urlOf(AbstractDocument* document) const
{
    AbstractModelSynchronizer* synchronizer = document->synchronizer();

    return synchronizer ? synchronizer->url() : QUrl();
}

void DocumentSyncManagerPrivate::setDocumentSynchronizerFactory(AbstractModelSynchronizerFactory* synchronizerFactory)
{
    mSynchronizerFactory = synchronizerFactory;
}

void DocumentSyncManagerPrivate::load(const QUrl& url)
{
    Q_Q(DocumentSyncManager);

    const auto loadedDocuments = mManager->documents();
    for (AbstractDocument* document : loadedDocuments) {
        if (url == urlOf(document)) {
            // TODO: query if file should be reloaded/synched from disk
            emit mManager->focusRequested(document);
            return;
        }
    }

    AbstractModelSynchronizer* synchronizer = mSynchronizerFactory->createSynchronizer();
    AbstractLoadJob* loadJob = synchronizer->startLoad(url);
    QObject::connect(loadJob, &AbstractLoadJob::documentLoaded,
                     q, [this](AbstractDocument* document) { onDocumentLoaded(document); });

    JobManager::executeJob(loadJob);   // TODO: pass a ui handler to jobmanager

    // store path
//     mWorkingUrl = url.upUrl();
    emit q->urlUsed(url);
}

bool DocumentSyncManagerPrivate::setSynchronizer(AbstractDocument* document)
{
    Q_Q(DocumentSyncManager);

    bool storingDone = false;

    AbstractModelSynchronizer* currentSynchronizer = document->synchronizer();
    // TODO: warn if there were updates in the second before saveAs was activated
//     if( currentSynchronizer )
//         currentSynchronizer->pauseSynchronization(); also unpause below
    const QString processTitle =
        i18nc("@title:window", "Save As");
    do {
        QFileDialog dialog(/*mWidget*/ nullptr, processTitle, /*mWorkingUrl.url()*/ QString());
        dialog.setMimeTypeFilters(supportedRemoteTypes());
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        const QUrl newUrl = (dialog.exec() != 0) ? dialog.selectedUrls().value(0) : QUrl();

        if (newUrl.isValid()) {
            const bool isNewUrl = (!currentSynchronizer)
                                  || (newUrl != currentSynchronizer->url());

            if (isNewUrl) {
                KIO::StatJob* statJob = KIO::stat(newUrl);
                statJob->setSide(KIO::StatJob::DestinationSide);
                KJobWidgets::setWindow(statJob, /*mWidget*/ nullptr);

                const bool isUrlInUse = statJob->exec();

                if (isUrlInUse) {
                    // TODO: care for case that file from url is already loaded by (only?) this program
//                     const bool otherFileLoaded = mManager->documentByUrl( newUrl );
                    // TODO: replace "file" with synchronizer->storageTypeName() or such
                    // TODO: offer "Synchronize" as alternative, if supported, see below
                    // ask synchronizer for capabilities, as some can only overwrite
                    const Answer answer =
                        mOverwriteDialog ? mOverwriteDialog->queryOverwrite(newUrl, processTitle) : Cancel;

                    if (answer == Cancel) {
                        break;
                    }
                    if (answer == PreviousQuestion) {
                        continue;
                    }
                }

                // switch url and synchronizer
                if (currentSynchronizer && true) {// TODO: same remote mimetype
                    // TODO: overwrite for now
                    AbstractSyncWithRemoteJob* syncJob = currentSynchronizer->startSyncWithRemote(newUrl,
                                                                                                  AbstractModelSynchronizer::ReplaceRemote);
                    const bool syncSucceeded = JobManager::executeJob(syncJob);
//                     currentSynchronizer->unpauseSynchronization(); also pause above
                    storingDone = syncSucceeded;
                } else {
                    // TODO: is overwrite for now, is this useful?
                    AbstractModelSynchronizer* synchronizer = mSynchronizerFactory->createSynchronizer();
                    AbstractConnectJob* connectJob = synchronizer->startConnect(document, newUrl,
                                                                                AbstractModelSynchronizer::ReplaceRemote);
                    const bool connectSucceeded = JobManager::executeJob(connectJob);

                    storingDone = connectSucceeded;
                }

                if (storingDone) {
                    emit q->urlUsed(newUrl);
                }
#if 0
//     mWorkingUrl = Url.upUrl();
                OpenRecentAction->addUrl(Url);
#endif
            }
            // same url
            else {
                // TODO: what to do? synchTo? synchWith? synchFrom? Or does the synchronizer care for this?
                // By e.g. warning that we might be overwriting something?
                // synchTo might be the intention, after all the user wanted a new storage
                //
                AbstractSyncToRemoteJob* syncJob = document->synchronizer()->startSyncToRemote();
                const bool syncFailed = JobManager::executeJob(syncJob);

                storingDone = !syncFailed;
            }
        } else {
            break;
        }
    } while (!storingDone);

    return storingDone;
}

bool DocumentSyncManagerPrivate::canClose(AbstractDocument* document)
{
    bool canClose = true;

    if (document->contentFlags() & ContentHasUnstoredChanges) {
        AbstractModelSynchronizer* synchronizer = document->synchronizer();
        const bool couldSynchronize = hasSynchronizerForLocal(document->mimeType());

        const QString processTitle = i18nc("@title:window", "Close");

        if ((synchronizer && synchronizer->localSyncState() == LocalHasChanges) ||
            couldSynchronize) {
            const Answer answer =
                mSaveDiscardDialog ? mSaveDiscardDialog->querySaveDiscard(document, processTitle) : Cancel;

            if (answer == Save) {
                if (synchronizer) {
                    AbstractSyncToRemoteJob* syncJob = synchronizer->startSyncToRemote();
                    const bool isSynced = JobManager::executeJob(syncJob);

                    canClose = isSynced;
                } else {
                    canClose = setSynchronizer(document);
                }
            } else {
                canClose = (answer == Discard);
            }
        } else {
            const Answer answer =
                mSaveDiscardDialog ? mSaveDiscardDialog->queryDiscard(document, processTitle) : Cancel;

            canClose = (answer == Discard);
        }
    }

    return canClose;
}

void DocumentSyncManagerPrivate::reload(AbstractDocument* document)
{
    AbstractModelSynchronizer* synchronizer = document->synchronizer();

    if (synchronizer->localSyncState() == LocalHasChanges) {
        const QString processTitle = i18nc("@title:window", "Reload");

        const Answer answer =
            mSaveDiscardDialog ? mSaveDiscardDialog->queryDiscardOnReload(document, processTitle) : Cancel;

        if (answer == Cancel) {
            return;
        }
    }

    AbstractSyncFromRemoteJob* syncJob = synchronizer->startSyncFromRemote();
    JobManager::executeJob(syncJob);
}

void DocumentSyncManagerPrivate::save(AbstractDocument* document)
{
    AbstractModelSynchronizer* synchronizer = document->synchronizer();
    AbstractSyncToRemoteJob* syncJob = synchronizer->startSyncToRemote();
    JobManager::executeJob(syncJob);
}

void DocumentSyncManagerPrivate::onDocumentLoaded(AbstractDocument* document)
{
    if (document) {
        mManager->addDocument(document);
    }
}

}
