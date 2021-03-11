/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMCONNECTJOB_HPP
#define KASTEN_ABSTRACTFILESYSTEMCONNECTJOB_HPP

// library
#include "abstractconnectjob.hpp"
#include "abstractmodelsynchronizer.hpp"

class QFile;

namespace Kasten {

class AbstractModelFileSystemSynchronizer;

class AbstractFileSystemConnectJobPrivate;

class KASTENCORE_EXPORT AbstractFileSystemConnectJob : public AbstractConnectJob
{
    Q_OBJECT

public:
    AbstractFileSystemConnectJob(AbstractModelFileSystemSynchronizer* synchronizer, AbstractDocument* document,
                                 const QUrl& url, AbstractModelSynchronizer::ConnectOption option);

    ~AbstractFileSystemConnectJob() override;

public: // KJob API
    void start() override;

protected: // API to be implemented
    virtual void startConnectWithFile() = 0;

protected:
    void complete(bool success);

protected:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    AbstractDocument* document() const;
    QFile* file() const;

private:
    Q_PRIVATE_SLOT(d_func(), void connectWithFile())

private:
    Q_DECLARE_PRIVATE(AbstractFileSystemConnectJob)
};

}

#endif
