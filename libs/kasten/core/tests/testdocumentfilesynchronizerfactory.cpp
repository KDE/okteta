/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilesynchronizerfactory.hpp"

// lib
#include "testdocument.hpp"
#include "testdocumentfileconnectjob.hpp"
#include "testdocumentfileloadjob.hpp"
#include "testdocumentfilesynchronizer.hpp"
// Qt
#include <QString>
#include <QUrl>

namespace Kasten {

TestDocumentFileSynchronizerFactory::TestDocumentFileSynchronizerFactory(const QByteArray& header)
    : mHeader(header)
{}

TestDocumentFileSynchronizerFactory::~TestDocumentFileSynchronizerFactory() = default;

// could be set to base class as value, is only one object per factory at runtime
// and settable in desktop file
QString TestDocumentFileSynchronizerFactory::supportedWorkType() const { return QStringLiteral("TestDocument");}
QString TestDocumentFileSynchronizerFactory::supportedRemoteType() const { return QStringLiteral("application/octet-stream");}

AbstractLoadJob* TestDocumentFileSynchronizerFactory::startLoad(const QUrl& url)
{
    auto synchronizer = std::make_unique<TestDocumentFileSynchronizer>(mHeader);
    return new TestDocumentFileLoadJob(std::move(synchronizer), url);
}

AbstractConnectJob* TestDocumentFileSynchronizerFactory::startConnect(AbstractDocument* document,
                                                                      const QUrl& url,
                                                                      AbstractModelSynchronizer::ConnectOption option)
{
    auto synchronizer = std::make_unique<TestDocumentFileSynchronizer>(mHeader);
    return new TestDocumentFileConnectJob(std::move(synchronizer), document, url, option);
}

}

#include "moc_testdocumentfilesynchronizerfactory.cpp"
