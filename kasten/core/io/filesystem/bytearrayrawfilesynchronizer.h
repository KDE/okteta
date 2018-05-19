/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYRAWFILESYNCHRONIZER_H
#define BYTEARRAYRAWFILESYNCHRONIZER_H

// lib
#include <kasten/okteta/oktetakastencore_export.h>
// Kasten core
#include <kasten/abstractmodelfilesystemsynchronizer.h>

namespace Kasten {

class ByteArrayDocument;

class OKTETAKASTENCORE_EXPORT ByteArrayRawFileSynchronizer : public AbstractModelFileSystemSynchronizer
{
    Q_OBJECT

    friend class ByteArrayRawFileLoadJob;
    friend class ByteArrayRawFileConnectJob;

public:
    ByteArrayRawFileSynchronizer();

public: // AbstractModelSynchronizer API
    AbstractLoadJob* startLoad(const QUrl& url) override;
    AbstractSyncToRemoteJob* startSyncToRemote() override;
    AbstractSyncFromRemoteJob* startSyncFromRemote() override;
    AbstractSyncWithRemoteJob* startSyncWithRemote(const QUrl& url, AbstractModelSynchronizer::ConnectOption option) override;
    AbstractConnectJob* startConnect(AbstractDocument* document,
                                     const QUrl& url, AbstractModelSynchronizer::ConnectOption option) override;

    AbstractDocument* document() const override;

    LocalSyncState localSyncState() const override;

protected:
    void setDocument(ByteArrayDocument* document);

private Q_SLOTS:
    void onUrlChange(const QUrl& url);
    void onModelModified(bool isModified);

private:
    ByteArrayDocument* mDocument;
};

}

#endif
