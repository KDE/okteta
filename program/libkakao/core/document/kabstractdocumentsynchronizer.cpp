/***************************************************************************
                          kabstractdocumentsynchronizer.cpp  -  description
                             -------------------
    begin                : Mon Nov 12 2007
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


#include "kabstractdocumentsynchronizer.h"

// KDE
#include <KUrl>

class KAbstractDocumentSynchronizer::Private
{
  public:
    KUrl url() const;
    void setUrl( const KUrl &url );
  protected:
    KUrl mUrl;
};

inline KUrl KAbstractDocumentSynchronizer::Private::url() const { return mUrl; }
inline void KAbstractDocumentSynchronizer::Private::setUrl( const KUrl &url) { mUrl = url; }


KAbstractDocumentSynchronizer::KAbstractDocumentSynchronizer()
: d( new Private() )
{}

KUrl KAbstractDocumentSynchronizer::url() const { return d->url(); }
void KAbstractDocumentSynchronizer::setUrl( const KUrl &url )
{
    d->setUrl( url );
    emit urlChanged( url );
}

KAbstractDocumentSynchronizer::~KAbstractDocumentSynchronizer()
{
    delete d;
}
