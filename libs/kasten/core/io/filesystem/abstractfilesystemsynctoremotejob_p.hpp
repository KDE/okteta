/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMSYNCTOREMOTEJOB_P_HPP
#define KASTEN_ABSTRACTFILESYSTEMSYNCTOREMOTEJOB_P_HPP

#include "abstractfilesystemsynctoremotejob.hpp"
#include <abstractsynctoremotejob_p.hpp>

// library
#include "abstractmodelfilesystemsynchronizer.hpp"
// Qt
#include <QUrl>
#include <QTemporaryFile>

namespace Kasten {
class AbstractFileSystemSyncToRemoteJobPrivate : public AbstractSyncToRemoteJobPrivate
{
public:
    AbstractFileSystemSyncToRemoteJobPrivate(AbstractFileSystemSyncToRemoteJob* parent, AbstractModelFileSystemSynchronizer* synchronizer);
    AbstractFileSystemSyncToRemoteJobPrivate() = delete;

    ~AbstractFileSystemSyncToRemoteJobPrivate() override;

public: // KJob API
    void start();

public:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    QFile* file() const;

public:
    void completeWrite(bool success);

public: // slots
    void syncToRemote();

protected:
    AbstractModelFileSystemSynchronizer* const mSynchronizer;
    QFile* mFile = nullptr;
    QString mWorkFilePath;

private:
    Q_DECLARE_PUBLIC(AbstractFileSystemSyncToRemoteJob)
};

inline AbstractFileSystemSyncToRemoteJobPrivate::AbstractFileSystemSyncToRemoteJobPrivate(AbstractFileSystemSyncToRemoteJob* parent,
                                                                                          AbstractModelFileSystemSynchronizer* synchronizer)
    : AbstractSyncToRemoteJobPrivate(parent)
    , mSynchronizer(synchronizer)
{}

inline QFile* AbstractFileSystemSyncToRemoteJobPrivate::file()     const { return mFile; }
// TODO: setup a notification system
inline AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncToRemoteJobPrivate::synchronizer() const
{
    return mSynchronizer;
}

inline void AbstractFileSystemSyncToRemoteJobPrivate::start()
{
    Q_Q(AbstractFileSystemSyncToRemoteJob);

    QMetaObject::invokeMethod(q, "syncToRemote", Qt::QueuedConnection);
}

}

#endif
