/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILESYNCHRONIZER_HPP
#define KASTEN_BYTEARRAYRAWFILESYNCHRONIZER_HPP

// lib
#include "oktetakastencore_export.hpp"
// Kasten core
#include <Kasten/AbstractModelFileSystemSynchronizer>

namespace Kasten {

class ByteArrayDocument;

class OKTETAKASTENCORE_EXPORT ByteArrayRawFileSynchronizer : public AbstractModelFileSystemSynchronizer
{
    Q_OBJECT

    friend class ByteArrayRawFileLoadJob;
    friend class ByteArrayRawFileConnectJob;

public:
    ByteArrayRawFileSynchronizer();

public: // AbstractModelSynchronizer API
    AbstractLoadJob* startLoad(const QUrl& url) override;
    AbstractSyncToRemoteJob* startSyncToRemote() override;
    AbstractSyncFromRemoteJob* startSyncFromRemote() override;
    AbstractSyncWithRemoteJob* startSyncWithRemote(const QUrl& url, AbstractModelSynchronizer::ConnectOption option) override;
    AbstractConnectJob* startConnect(AbstractDocument* document,
                                     const QUrl& url, AbstractModelSynchronizer::ConnectOption option) override;

    AbstractDocument* document() const override;

    LocalSyncState localSyncState() const override;

protected:
    void setDocument(ByteArrayDocument* document);

private Q_SLOTS:
    void onUrlChange(const QUrl& url);
    void onModelModified(bool isModified);

private:
    ByteArrayDocument* mDocument = nullptr;
};

}

#endif
