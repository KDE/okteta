/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELSYNCHRONIZERFACTORY_HPP
#define KASTEN_ABSTRACTMODELSYNCHRONIZERFACTORY_HPP

// lib
#include "abstractmodelsynchronizer.hpp"
// Qt
#include <QObject>

namespace Kasten {

// not really a classic factory, functions do some more, instead it is what?
// TODO: improve the whole vodoo, especially linking document and synchronizer
// make it foolproof
// make a simple tutorial with an example program
// TODO: should the load/connect/export jobs be created here or from the synchronizer?
// putting jobs to synchronizer keeps factory simple, but forces synchronizer to be
// in invalid states, like with simple constructor
class KASTENCORE_EXPORT AbstractModelSynchronizerFactory : public QObject
{
    Q_OBJECT

public:
    ~AbstractModelSynchronizerFactory() override;

public: // API to be implemented
    virtual AbstractModelSynchronizer* createSynchronizer() const = 0;

//     virtual AbstractDocument* loadNewDocument( const QUrl& originUrl ) const = 0;
//     virtual AbstractLoadJob* startLoad( const QUrl& url ) = 0;
    // TODO: better name than connect: bind?

//     virtual bool connectDocument( AbstractDocument* document, const QUrl& originUrl,
//                                   AbstractDocumentSynchronizer::ConnectOption option ) const = 0;
//     virtual AbstractConnectJob* startConnect( AbstractDocument* document,
//                                               const QUrl& url, AbstractDocumentSynchronizer::ConnectOption option ) = 0;

//     virtual bool exportDocument( AbstractDocument* document, const QUrl& originUrl ) const = 0;
//     virtual AbstractExportJob* startExport( AbstractDocument* document, const QUrl& originUrl ) const = 0;

    /** returns the id of the work model type */
    // TODO: is QByteArray enough?
    virtual QString supportedWorkType() const = 0;
    /** returns the id of the remote model type */
    virtual QString supportedRemoteType() const = 0;
};

}

#endif
