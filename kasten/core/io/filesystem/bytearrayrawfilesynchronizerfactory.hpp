/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRAWFILESYNCHRONIZERFACTORY_HPP
#define KASTEN_BYTEARRAYRAWFILESYNCHRONIZERFACTORY_HPP

// lib
#include "oktetakastencore_export.hpp"
// Kasten core
#include <Kasten/AbstractModelSynchronizerFactory>

namespace Kasten {

class OKTETAKASTENCORE_EXPORT ByteArrayRawFileSynchronizerFactory : public AbstractModelSynchronizerFactory
{
    Q_OBJECT

public:
    ByteArrayRawFileSynchronizerFactory();
    ~ByteArrayRawFileSynchronizerFactory() override;

public: // AbstractModelSynchronizerFactory API
    [[nodiscard]]
    AbstractLoadJob* startLoad(const QUrl& url) override;
    [[nodiscard]]
    AbstractConnectJob* startConnect(AbstractDocument* document,
                                     const QUrl& url, AbstractModelSynchronizer::ConnectOption option) override;

    [[nodiscard]]
    QString supportedWorkType() const override;
    [[nodiscard]]
    QString supportedRemoteType() const override;
};

}

#endif
