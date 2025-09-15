/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILEWRITEJOB_HPP
#define KASTEN_BYTEARRAYRAWFILEWRITEJOB_HPP

// Kasten core
#include <Kasten/AbstractFileSystemSyncToRemoteJob>

namespace Kasten {

class ByteArrayRawFileSynchronizer;

class ByteArrayRawFileWriteJob : public AbstractFileSystemSyncToRemoteJob
{
    Q_OBJECT

public:
    explicit ByteArrayRawFileWriteJob(ByteArrayRawFileSynchronizer* synchronizer);
    ~ByteArrayRawFileWriteJob() override;

protected: // AbstractFileSystemSyncToRemoteJob API
    void startWriteToFile() override;

private Q_SLOTS:
//     void onDataPulled();
};

}

#endif
