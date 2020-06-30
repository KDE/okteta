/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilesynchronizer.hpp"

// lib
#include "testdocumentfileloadjob.hpp"
#include "testdocumentfileconnectjob.hpp"
#include "testdocumentfilewritejob.hpp"
#include "testdocumentfilereloadjob.hpp"
#include "testdocumentfilewritetojob.hpp"
#include "testdocument.hpp"
// Qt
#include <QUrl>

namespace Kasten {

TestDocumentFileSynchronizer::TestDocumentFileSynchronizer(const QByteArray& header)
    : mHeader(header)
{
    // TODO: where to catch this? who decides about this?
//     mDocument->setTitle( url.fileName() );
}

AbstractDocument* TestDocumentFileSynchronizer::document() const { return mDocument; }
LocalSyncState TestDocumentFileSynchronizer::localSyncState() const
{
    return mDocument ?
        ((mDocument->contentFlags() & ContentHasUnstoredChanges) ? LocalHasChanges : LocalInSync) : LocalInSync;
}

AbstractLoadJob* TestDocumentFileSynchronizer::startLoad(const QUrl& url)
{
    return new TestDocumentFileLoadJob(this, url);
}

AbstractSyncToRemoteJob* TestDocumentFileSynchronizer::startSyncToRemote()
{
    return new TestDocumentFileWriteJob(this);
}

AbstractSyncFromRemoteJob* TestDocumentFileSynchronizer::startSyncFromRemote()
{
    return new TestDocumentFileReloadJob(this);
}

AbstractSyncWithRemoteJob* TestDocumentFileSynchronizer::startSyncWithRemote(const QUrl& url, AbstractModelSynchronizer::ConnectOption option)
{
    return new TestDocumentFileWriteToJob(this, url, option);
}

AbstractConnectJob* TestDocumentFileSynchronizer::startConnect(AbstractDocument* document,
                                                               const QUrl& url, AbstractModelSynchronizer::ConnectOption option)
{
    return new TestDocumentFileConnectJob(this, document, url, option);
}

}
