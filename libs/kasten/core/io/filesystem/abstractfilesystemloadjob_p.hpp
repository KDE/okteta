/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMLOADJOB_P_HPP
#define KASTEN_ABSTRACTFILESYSTEMLOADJOB_P_HPP

// lib
#include "abstractfilesystemloadjob.hpp"
#include <abstractloadjob_p.hpp>

#include <QUrl>

namespace Kasten {

class AbstractFileSystemLoadJobPrivate : public AbstractLoadJobPrivate
{
public:
    AbstractFileSystemLoadJobPrivate(AbstractFileSystemLoadJob* parent, AbstractModelFileSystemSynchronizer* synchronizer, const QUrl& url);
    AbstractFileSystemLoadJobPrivate() = delete;

    ~AbstractFileSystemLoadJobPrivate() override;

public: // KJob API
    void start();

public: // AbstractLoadJob API
    void setDocument(AbstractDocument* document);

public:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    const QUrl& url() const;
    QFile* file() const;

public: // slots
    void load();

protected:
    AbstractModelFileSystemSynchronizer* const mSynchronizer;
    const QUrl mUrl;
    QFile* mFile = nullptr;
    QString mWorkFilePath;
    QString mTempFilePath;

private:
    Q_DECLARE_PUBLIC(AbstractFileSystemLoadJob)
};

inline AbstractFileSystemLoadJobPrivate::AbstractFileSystemLoadJobPrivate(AbstractFileSystemLoadJob* parent,
                                                                          AbstractModelFileSystemSynchronizer* synchronizer, const QUrl& url)
    : AbstractLoadJobPrivate(parent)
    , mSynchronizer(synchronizer)
    , mUrl(url)
{}

inline AbstractFileSystemLoadJobPrivate::~AbstractFileSystemLoadJobPrivate() = default;

inline AbstractModelFileSystemSynchronizer* AbstractFileSystemLoadJobPrivate::synchronizer() const
{
    return mSynchronizer;
}
inline const QUrl& AbstractFileSystemLoadJobPrivate::url() const { return mUrl; }

inline QFile* AbstractFileSystemLoadJobPrivate::file()     const { return mFile; }
// TODO: setup a notification system

inline void AbstractFileSystemLoadJobPrivate::start()
{
    Q_Q(AbstractFileSystemLoadJob);

    QMetaObject::invokeMethod(q, "load", Qt::QueuedConnection);
}

}

#endif
