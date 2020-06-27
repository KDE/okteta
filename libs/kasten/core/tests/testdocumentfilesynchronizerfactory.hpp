/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TESTDOCUMENTFILESYNCHRONIZERFACTORY_HPP
#define KASTEN_TESTDOCUMENTFILESYNCHRONIZERFACTORY_HPP

// Kasten
#include <abstractmodelsynchronizerfactory.hpp>
// Qt
#include <QByteArray>

namespace Kasten {

class TestDocumentFileSynchronizerFactory : public AbstractModelSynchronizerFactory
{
    Q_OBJECT

public:
    TestDocumentFileSynchronizerFactory(const QByteArray& header = QByteArray());
    ~TestDocumentFileSynchronizerFactory() override;

public: // AbstractModelSynchronizerFactory API
    AbstractModelSynchronizer* createSynchronizer() const override;

    QString supportedWorkType() const override;
    QString supportedRemoteType() const override;

private:
    QByteArray mHeader;
};

}

#endif
