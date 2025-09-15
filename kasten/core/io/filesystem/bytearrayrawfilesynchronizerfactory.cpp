/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfilesynchronizerfactory.hpp"

// lib
#include "bytearrayrawfilesynchronizer.hpp"
#include "bytearrayrawfileloadjob.hpp"
#include "bytearrayrawfileconnectjob.hpp"
#include "bytearraydocument.hpp"
// Qt
#include <QString>
#include <QUrl>

namespace Kasten {

ByteArrayRawFileSynchronizerFactory::ByteArrayRawFileSynchronizerFactory() = default;

ByteArrayRawFileSynchronizerFactory::~ByteArrayRawFileSynchronizerFactory() = default;

// could be set to base class as value, is only one object per factory at runtime
// and settable in desktop file
QString ByteArrayRawFileSynchronizerFactory::supportedWorkType() const { return QStringLiteral("ByteArrayDocument");}
QString ByteArrayRawFileSynchronizerFactory::supportedRemoteType() const { return QStringLiteral("application/octet-stream");}

AbstractLoadJob* ByteArrayRawFileSynchronizerFactory::startLoad(const QUrl& url)
{
    auto synchronizer = std::make_unique<ByteArrayRawFileSynchronizer>();
    return new ByteArrayRawFileLoadJob(std::move(synchronizer), url);
}

AbstractConnectJob* ByteArrayRawFileSynchronizerFactory::startConnect(AbstractDocument* document,
                                                                      const QUrl& url,
                                                                      AbstractModelSynchronizer::ConnectOption option)
{
    auto synchronizer = std::make_unique<ByteArrayRawFileSynchronizer>();
    return new ByteArrayRawFileConnectJob(std::move(synchronizer), document, url, option);
}

}

#include "moc_bytearrayrawfilesynchronizerfactory.cpp"
