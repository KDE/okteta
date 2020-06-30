/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILERELOADJOB_HPP
#define KASTEN_BYTEARRAYRAWFILERELOADJOB_HPP

// Kasten core
#include <Kasten/AbstractFileSystemSyncFromRemoteJob>

namespace Kasten {

class ByteArrayRawFileSynchronizer;
class ByteArrayRawFileReloadThread;

class ByteArrayRawFileReloadJob : public AbstractFileSystemSyncFromRemoteJob
{
    Q_OBJECT

public:
    explicit ByteArrayRawFileReloadJob(ByteArrayRawFileSynchronizer* synchronizer);
    ~ByteArrayRawFileReloadJob() override;

protected: // AbstractFileSystemSyncFromRemoteJob API
    void startReadFromFile() override;

private Q_SLOTS:
//     void onDataPulled();

private:
    ByteArrayRawFileReloadThread* mLoadThread;
};

}

#endif
