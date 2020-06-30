/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILELOADJOB_HPP
#define KASTEN_BYTEARRAYRAWFILELOADJOB_HPP

// Kasten core
#include <Kasten/AbstractFileSystemLoadJob>

namespace Kasten {

class ByteArrayRawFileSynchronizer;
class ByteArrayRawFileLoadThread;

class ByteArrayRawFileLoadJob : public AbstractFileSystemLoadJob
{
    Q_OBJECT

public:
    ByteArrayRawFileLoadJob(ByteArrayRawFileSynchronizer* synchronizer, const QUrl& url);
    ~ByteArrayRawFileLoadJob() override;

protected: // AbstractFileSystemLoadJob API
    void startLoadFromFile() override;

private Q_SLOTS:
//     void onDataPulled();

private:
    ByteArrayRawFileLoadThread* mLoadThread;
};

}

#endif
