/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfilesynchronizer.hpp"

// lib
#include "bytearrayrawfilewritejob.hpp"
#include "bytearrayrawfilereloadjob.hpp"
#include "bytearrayrawfilewritetojob.hpp"
#include "bytearraydocument.hpp"
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QUrl>

namespace Kasten {

ByteArrayRawFileSynchronizer::ByteArrayRawFileSynchronizer()
{
    connect(this, &ByteArrayRawFileSynchronizer::urlChanged, this, &ByteArrayRawFileSynchronizer::onUrlChange);
}

AbstractDocument* ByteArrayRawFileSynchronizer::document() const { return mDocument; }

LocalSyncState ByteArrayRawFileSynchronizer::localSyncState() const
{
    return mDocument ?
           (mDocument->content()->isModified() ? LocalHasChanges : LocalInSync) : LocalInSync;
}

void ByteArrayRawFileSynchronizer::setDocument(ByteArrayDocument* document)
{
    mDocument = document;
    if (mDocument) {
        connect(mDocument->content(), &Okteta::AbstractByteArrayModel::modifiedChanged,
                this, &ByteArrayRawFileSynchronizer::onModelModified);
    }
}

AbstractSyncToRemoteJob* ByteArrayRawFileSynchronizer::startSyncToRemote()
{
    return new ByteArrayRawFileWriteJob(this);
}

AbstractSyncFromRemoteJob* ByteArrayRawFileSynchronizer::startSyncFromRemote()
{
    return new ByteArrayRawFileReloadJob(this);
}

AbstractSyncWithRemoteJob* ByteArrayRawFileSynchronizer::startSyncWithRemote(const QUrl& url, AbstractModelSynchronizer::ConnectOption option)
{
    return new ByteArrayRawFileWriteToJob(this, url, option);
}

void ByteArrayRawFileSynchronizer::onUrlChange(const QUrl& url)
{
    mDocument->setTitle(url.fileName());
}

void ByteArrayRawFileSynchronizer::onModelModified(bool isModified)
{
    Q_EMIT localSyncStateChanged((isModified ? LocalHasChanges : LocalInSync));
}

}

#include "moc_bytearrayrawfilesynchronizer.cpp"
