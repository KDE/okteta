/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMSYNCWITHREMOTEJOB_P_HPP
#define KASTEN_ABSTRACTFILESYSTEMSYNCWITHREMOTEJOB_P_HPP

// library
#include "abstractfilesystemsyncwithremotejob.hpp"
#include <abstractsyncwithremotejob_p.hpp>

#include <QUrl>

namespace Kasten {

class AbstractFileSystemSyncWithRemoteJobPrivate : public AbstractSyncWithRemoteJobPrivate
{
public:
    AbstractFileSystemSyncWithRemoteJobPrivate(AbstractFileSystemSyncWithRemoteJob* parent,
                                               AbstractModelFileSystemSynchronizer* synchronizer,
                                               const QUrl& url, AbstractModelSynchronizer::ConnectOption option);
    AbstractFileSystemSyncWithRemoteJobPrivate() = delete;

    ~AbstractFileSystemSyncWithRemoteJobPrivate() override;

public: // KJob API
    void start();

protected:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    QFile* file() const;

protected:
    void completeSync(bool success);

protected: // slots
    void syncWithRemote();

protected:
    AbstractModelFileSystemSynchronizer* const mSynchronizer;
    const QUrl mUrl;
    const AbstractModelSynchronizer::ConnectOption mOption;
    QFile* mFile = nullptr;
    QString mWorkFilePath;
    QString mTempFilePath;

private:
    Q_DECLARE_PUBLIC(AbstractFileSystemSyncWithRemoteJob)
};

inline AbstractFileSystemSyncWithRemoteJobPrivate::AbstractFileSystemSyncWithRemoteJobPrivate(AbstractFileSystemSyncWithRemoteJob* parent,
                                                                                              AbstractModelFileSystemSynchronizer* synchronizer,
                                                                                              const QUrl& url, AbstractModelSynchronizer::ConnectOption option)
    : AbstractSyncWithRemoteJobPrivate(parent)
    , mSynchronizer(synchronizer)
    , mUrl(url)
    , mOption(option)
{}

inline AbstractFileSystemSyncWithRemoteJobPrivate::~AbstractFileSystemSyncWithRemoteJobPrivate() = default;

inline QFile* AbstractFileSystemSyncWithRemoteJobPrivate::file()     const { return mFile; }
// TODO: setup a notification system
inline AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncWithRemoteJobPrivate::synchronizer() const
{
    return mSynchronizer;
}

inline void AbstractFileSystemSyncWithRemoteJobPrivate::start()
{
    Q_Q(AbstractFileSystemSyncWithRemoteJob);

    QMetaObject::invokeMethod(q, "syncWithRemote", Qt::QueuedConnection);
}

}

#endif
