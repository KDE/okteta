/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILEWRITETOJOB_HPP
#define KASTEN_BYTEARRAYRAWFILEWRITETOJOB_HPP

// Kasten core
#include <Kasten/AbstractFileSystemSyncWithRemoteJob>

namespace Kasten {

class ByteArrayRawFileSynchronizer;
class ByteArrayRawFileWriteThread;

class ByteArrayRawFileWriteToJob : public AbstractFileSystemSyncWithRemoteJob
{
    Q_OBJECT

public:
    ByteArrayRawFileWriteToJob(ByteArrayRawFileSynchronizer* synchronizer,
                               const QUrl& url, AbstractModelSynchronizer::ConnectOption option);
    ~ByteArrayRawFileWriteToJob() override;

protected: // AbstractFileSystemSyncWithRemoteJob API
    void startSyncWithRemote() override;

private Q_SLOTS:
//     void onDataPulled();

private:
    ByteArrayRawFileWriteThread* mWriteThread;
};

}

#endif
