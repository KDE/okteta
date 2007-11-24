/***************************************************************************
                          kbytearrayrawfilesynchronizerfactory.h  -  description
                             -------------------
    begin                : Mon Nov 19 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KBYTEARRAYRAWFILESYNCHRONIZERFACTORY_H
#define KBYTEARRAYRAWFILESYNCHRONIZERFACTORY_H

// kakao
#include <kabstractdocumentsynchronizerfactory.h>

class KByteArrayRawFileSynchronizerFactory : public KAbstractDocumentSynchronizerFactory
{
  Q_OBJECT

  public:
    KByteArrayRawFileSynchronizerFactory();
    virtual ~KByteArrayRawFileSynchronizerFactory();

  public: // KAbstractDocumentSynchronizerFactory API
    virtual KAbstractDocument *loadNewDocument( const KUrl &originUrl ) const;
    // TODO: better name than connect: bind?
    virtual bool connectDocument( KAbstractDocument *document, const KUrl &originUrl,
                                  KAbstractDocumentSynchronizer::ConnectOption option ) const;
    virtual bool exportDocument( KAbstractDocument *document, const KUrl &originUrl ) const;

    virtual QString supportedWorkType() const;
    virtual QString supportedRemoteType() const;
};

#endif
