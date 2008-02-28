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

#ifndef TESTDOCUMENTFILESYNCHRONIZERFACTORY_H
#define TESTDOCUMENTFILESYNCHRONIZERFACTORY_H

// Kakao
#include <kabstractdocumentsynchronizerfactory.h>
// Qt
#include <QtCore/QByteArray>

class TestDocumentFileSynchronizerFactory : public KAbstractDocumentSynchronizerFactory
{
  Q_OBJECT

  public:
    TestDocumentFileSynchronizerFactory( const QByteArray &header = QByteArray() );
    virtual ~TestDocumentFileSynchronizerFactory();

  public: // KAbstractDocumentSynchronizerFactory API
    virtual KAbstractDocument *loadNewDocument( const KUrl &originUrl ) const;
    // TODO: better name than connect: bind?
    virtual bool connectDocument( KAbstractDocument *document, const KUrl &originUrl,
                                  KAbstractDocumentSynchronizer::ConnectOption option ) const;
    virtual bool exportDocument( KAbstractDocument *document, const KUrl &originUrl ) const;

    virtual QString supportedWorkType() const;
    virtual QString supportedRemoteType() const;

  protected:
    QByteArray mHeader;
};

#endif
