/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMSYNCWITHREMOTEJOB_HPP
#define KASTEN_ABSTRACTFILESYSTEMSYNCWITHREMOTEJOB_HPP

// library
#include "abstractsyncwithremotejob.hpp"
#include "abstractmodelsynchronizer.hpp"

class QUrl;
class QFile;

namespace Kasten {

class AbstractModelFileSystemSynchronizer;

class AbstractFileSystemSyncWithRemoteJobPrivate;

class KASTENCORE_EXPORT AbstractFileSystemSyncWithRemoteJob : public AbstractSyncWithRemoteJob
{
    Q_OBJECT

public:
    AbstractFileSystemSyncWithRemoteJob(AbstractModelFileSystemSynchronizer* synchronizer,
                                        const QUrl& url, AbstractModelSynchronizer::ConnectOption option);
    ~AbstractFileSystemSyncWithRemoteJob() override;

public: // KJob API
    void start() override;

protected: // API to be implemented
    virtual void startSyncWithRemote() = 0;

protected:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    QFile* file() const;

protected:
    void completeSync(bool success);

private:
    Q_PRIVATE_SLOT(d_func(), void syncWithRemote())

private:
    Q_DECLARE_PRIVATE(AbstractFileSystemSyncWithRemoteJob)
};

}

#endif
