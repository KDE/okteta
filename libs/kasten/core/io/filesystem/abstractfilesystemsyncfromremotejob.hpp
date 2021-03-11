/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMSYNCFROMREMOTEJOB_HPP
#define KASTEN_ABSTRACTFILESYSTEMSYNCFROMREMOTEJOB_HPP

// library
#include "abstractsyncfromremotejob.hpp"

class QFile;

namespace Kasten {

class AbstractModelFileSystemSynchronizer;

class AbstractFileSystemSyncFromRemoteJobPrivate;

class KASTENCORE_EXPORT AbstractFileSystemSyncFromRemoteJob : public AbstractSyncFromRemoteJob
{
    Q_OBJECT

public:
    explicit AbstractFileSystemSyncFromRemoteJob(AbstractModelFileSystemSynchronizer* synchronizer);

    ~AbstractFileSystemSyncFromRemoteJob() override;

public: // KJob API
    void start() override;

protected: // API to be implemented
    virtual void startReadFromFile() = 0;

protected:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    QFile* file() const;

protected:
    void completeRead(bool success);

private:
    Q_PRIVATE_SLOT(d_func(), void syncFromRemote())

private:
    Q_DECLARE_PRIVATE(AbstractFileSystemSyncFromRemoteJob)
};

}

#endif
