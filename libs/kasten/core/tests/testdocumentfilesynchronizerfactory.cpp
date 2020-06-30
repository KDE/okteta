/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilesynchronizerfactory.hpp"

// lib
#include "testdocument.hpp"
#include "testdocumentfilesynchronizer.hpp"
// Qt
#include <QString>

namespace Kasten {

TestDocumentFileSynchronizerFactory::TestDocumentFileSynchronizerFactory(const QByteArray& header)
    : mHeader(header)
{}

TestDocumentFileSynchronizerFactory::~TestDocumentFileSynchronizerFactory() = default;

// could be set to base class as value, is only one object per factory at runtime
// and settable in desktop file
QString TestDocumentFileSynchronizerFactory::supportedWorkType() const { return QStringLiteral("TestDocument");}
QString TestDocumentFileSynchronizerFactory::supportedRemoteType() const { return QStringLiteral("application/octet-stream");}

// TODO: these function seems to be always the same. Make macro or template function
// or, if there is only one place which calls this, move there
AbstractModelSynchronizer* TestDocumentFileSynchronizerFactory::createSynchronizer() const
{
    return new TestDocumentFileSynchronizer(mHeader);
}

}
