/*
    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMCONNECTJOB_P_HPP
#define KASTEN_ABSTRACTFILESYSTEMCONNECTJOB_P_HPP

// library
#include "abstractfilesystemconnectjob.hpp"
#include <abstractconnectjob_p.hpp>
#include "abstractmodelfilesystemsynchronizer.hpp"
// Qt
#include <QFile>
#include <QUrl>
// Std
#include <memory>

namespace Kasten {

class AbstractFileSystemConnectJobPrivate : public AbstractConnectJobPrivate
{
public:
    AbstractFileSystemConnectJobPrivate(AbstractFileSystemConnectJob* parent,
                                        std::unique_ptr<AbstractModelFileSystemSynchronizer>&& synchronizer,
                                        AbstractDocument* document,
                                        const QUrl& url, AbstractModelSynchronizer::ConnectOption option);

    ~AbstractFileSystemConnectJobPrivate() override;

public: // KJob API
    void start();

public:
    void complete(bool success);

public:
    [[nodiscard]]
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    [[nodiscard]]
    AbstractDocument* document() const;
    [[nodiscard]]
    QFile* file() const;

public:
    void connectWithFile();

protected:
    std::unique_ptr<AbstractModelFileSystemSynchronizer> mSynchronizer;
    AbstractDocument* const mDocument;
    const QUrl mUrl;
    const AbstractModelSynchronizer::ConnectOption mOption;
    std::unique_ptr<QFile> mFile ;
    QString mWorkFilePath;
    QString mTempFilePath;

private:
    Q_DECLARE_PUBLIC(AbstractFileSystemConnectJob)
};

inline AbstractFileSystemConnectJobPrivate::AbstractFileSystemConnectJobPrivate(AbstractFileSystemConnectJob* parent,
                                                                                std::unique_ptr<AbstractModelFileSystemSynchronizer>&& synchronizer, AbstractDocument* document,
                                                                                const QUrl& url, AbstractModelSynchronizer::ConnectOption option)
    : AbstractConnectJobPrivate(parent)
    , mSynchronizer(std::move(synchronizer))
    , mDocument(document)
    , mUrl(url)
    , mOption(option)
{}

inline AbstractFileSystemConnectJobPrivate::~AbstractFileSystemConnectJobPrivate() = default;

inline AbstractModelFileSystemSynchronizer* AbstractFileSystemConnectJobPrivate::synchronizer() const
{
    return mSynchronizer.get();
}
inline AbstractDocument* AbstractFileSystemConnectJobPrivate::document()   const { return mDocument; }
inline QFile* AbstractFileSystemConnectJobPrivate::file()                  const { return mFile.get(); }
// TODO: setup a notification system

inline void AbstractFileSystemConnectJobPrivate::start()
{
    Q_Q(AbstractFileSystemConnectJob);

    QMetaObject::invokeMethod(q, "connectWithFile", Qt::QueuedConnection);
}

}

#endif
