/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008-2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMCONNECTJOB_HPP
#define KASTEN_ABSTRACTFILESYSTEMCONNECTJOB_HPP

// library
#include <kasten/abstractconnectjob.hpp>
#include <kasten/abstractmodelsynchronizer.hpp>

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

protected:
    Q_PRIVATE_SLOT(d_func(), void connectWithFile())

protected:
    Q_DECLARE_PRIVATE(AbstractFileSystemConnectJob)
};

}

#endif
