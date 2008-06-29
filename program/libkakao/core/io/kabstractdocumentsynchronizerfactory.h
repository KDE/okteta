/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#ifndef KABSTRACTDOCUMENTSYNCHRONIZERFACTORY_H
#define KABSTRACTDOCUMENTSYNCHRONIZERFACTORY_H

// lib
#include "kabstractdocumentsynchronizer.h"
// Qt
#include <QtCore/QObject>

// not really a classic factory, functions do some more, instead it is what?
// TODO: improve the whole vodoo, especially linking document and synchronizer
// make it foolproof
// make a simple tutorial with an example program
// TODO: should the load/connect/export jobs be created here or from the synchronizer?
// putting jobs to synchronizer keeps factory simple, but forces synchronizer to be
// in invalid states, like with simple constructor
class KAbstractDocumentSynchronizerFactory : public QObject
{
  Q_OBJECT

  public:
    virtual ~KAbstractDocumentSynchronizerFactory();

  public: // API to be implemented
    virtual KAbstractDocumentSynchronizer *createSynchronizer() const = 0;

//     virtual KAbstractDocument *loadNewDocument( const KUrl &originUrl ) const = 0;
//     virtual AbstractLoadJob *startLoad( const KUrl &url ) = 0;
    // TODO: better name than connect: bind?

//     virtual bool connectDocument( KAbstractDocument *document, const KUrl &originUrl,
//                                   KAbstractDocumentSynchronizer::ConnectOption option ) const = 0;
//     virtual AbstractConnectJob *startConnect( KAbstractDocument *document,
//                                               const KUrl &url, KAbstractDocumentSynchronizer::ConnectOption option ) = 0;

//     virtual bool exportDocument( KAbstractDocument *document, const KUrl &originUrl ) const = 0;
//     virtual AbstractExportJob *startExport( KAbstractDocument *document, const KUrl &originUrl ) const = 0;

    /** returns the id of the work model type */
    // TODO: is QByteArray enough?
    virtual QString supportedWorkType() const = 0;
    /** returns the id of the remote model type */
    virtual QString supportedRemoteType() const = 0;
};

#endif
