/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMSYNCTOREMOTEJOB_HPP
#define KASTEN_ABSTRACTFILESYSTEMSYNCTOREMOTEJOB_HPP

// library
#include "abstractsynctoremotejob.hpp"

class QFile;

namespace Kasten {

class AbstractModelFileSystemSynchronizer;

class AbstractFileSystemSyncToRemoteJobPrivate;

class KASTENCORE_EXPORT AbstractFileSystemSyncToRemoteJob : public AbstractSyncToRemoteJob
{
    Q_OBJECT

public:
    explicit AbstractFileSystemSyncToRemoteJob(AbstractModelFileSystemSynchronizer* synchronizer);

    ~AbstractFileSystemSyncToRemoteJob() override;

public: // KJob API
    void start() override;

protected: // API to be implemented
    virtual void startWriteToFile() = 0;

protected:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    QFile* file() const;

protected:
    void completeWrite(bool success);

private:
    Q_PRIVATE_SLOT(d_func(), void syncToRemote())

private:
    Q_DECLARE_PRIVATE(AbstractFileSystemSyncToRemoteJob)
};

}

#endif
