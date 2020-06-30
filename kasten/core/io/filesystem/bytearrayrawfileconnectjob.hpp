/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILECONNECTJOB_HPP
#define KASTEN_BYTEARRAYRAWFILECONNECTJOB_HPP

// Kasten core
#include <Kasten/AbstractFileSystemConnectJob>

namespace Kasten {

class ByteArrayRawFileSynchronizer;
class ByteArrayDocument;
// class ByteArrayRawFileWriteThread;

class ByteArrayRawFileConnectJob : public AbstractFileSystemConnectJob
{
    Q_OBJECT

public:
    ByteArrayRawFileConnectJob(ByteArrayRawFileSynchronizer* synchronizer, AbstractDocument* document,
                               const QUrl& url, AbstractModelSynchronizer::ConnectOption option);
    ~ByteArrayRawFileConnectJob() override;

protected: // AbstractFileSystemLoadJob API
    void startConnectWithFile() override;

private Q_SLOTS:
//     void onDataPulled();

private:
//     ByteArrayRawFileWriteThread *mWriteThread;
};

}

#endif
