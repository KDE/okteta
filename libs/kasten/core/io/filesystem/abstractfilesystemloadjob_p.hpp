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
#include "abstractmodelfilesystemsynchronizer.hpp"
// Qt
#include <QFile>
#include <QUrl>
// Std
#include <memory>

namespace Kasten {

class AbstractFileSystemLoadJobPrivate : public AbstractLoadJobPrivate
{
public:
    AbstractFileSystemLoadJobPrivate(AbstractFileSystemLoadJob* parent,
                                     std::unique_ptr<AbstractModelFileSystemSynchronizer>&& synchronizer, const QUrl& url);

    ~AbstractFileSystemLoadJobPrivate() override;

public: // KJob API
    void start();

public: // AbstractLoadJob API
    void setDocument(std::unique_ptr<AbstractDocument>&& document);

public:
    [[nodiscard]]
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    [[nodiscard]]
    const QUrl& url() const;
    [[nodiscard]]
    QFile* file() const;

public: // slots
    void load();

protected:
    std::unique_ptr<AbstractModelFileSystemSynchronizer> mSynchronizer;
    const QUrl mUrl;
    std::unique_ptr<QFile> mFile;
    QString mWorkFilePath;
    QString mTempFilePath;

private:
    Q_DECLARE_PUBLIC(AbstractFileSystemLoadJob)
};

inline AbstractFileSystemLoadJobPrivate::AbstractFileSystemLoadJobPrivate(AbstractFileSystemLoadJob* parent,
                                                                          std::unique_ptr<AbstractModelFileSystemSynchronizer>&& synchronizer, const QUrl& url)
    : AbstractLoadJobPrivate(parent)
    , mSynchronizer(std::move(synchronizer))
    , mUrl(url)
{}

inline AbstractFileSystemLoadJobPrivate::~AbstractFileSystemLoadJobPrivate() = default;

inline AbstractModelFileSystemSynchronizer* AbstractFileSystemLoadJobPrivate::synchronizer() const
{
    return mSynchronizer.get();
}
inline const QUrl& AbstractFileSystemLoadJobPrivate::url() const { return mUrl; }

inline QFile* AbstractFileSystemLoadJobPrivate::file()     const { return mFile.get(); }
// TODO: setup a notification system

inline void AbstractFileSystemLoadJobPrivate::start()
{
    Q_Q(AbstractFileSystemLoadJob);

    QMetaObject::invokeMethod(q, "load", Qt::QueuedConnection);
}

}

#endif
