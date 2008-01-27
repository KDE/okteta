/***************************************************************************
                          kabstractdocument.cpp  -  description
                             -------------------
    begin                : Mon May 21 2007
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


#include "kabstractdocument.h"

// lib
#include "kabstractdocumentsynchronizer.h"


class KAbstractDocument::Private
{
  public:
      explicit Private( KAbstractDocument *parent );
      ~Private();
  public:
    void setSynchronizer( KAbstractDocumentSynchronizer *synchronizer );
    KAbstractDocumentSynchronizer *synchronizer() const;
  protected:
     KAbstractDocument *d;
     KAbstractDocumentSynchronizer *mSynchronizer; // TODO: should this be here, with public setters and getters?
};

inline KAbstractDocument::Private::Private( KAbstractDocument *parent ) : d( parent ), mSynchronizer( 0 ) {}
inline KAbstractDocumentSynchronizer *KAbstractDocument::Private::synchronizer() const { return mSynchronizer; }
inline void KAbstractDocument::Private::setSynchronizer( KAbstractDocumentSynchronizer *synchronizer )
{
    // plugging the same more than once?
    if( mSynchronizer == synchronizer )
        return;

    delete mSynchronizer;
    mSynchronizer = synchronizer;
    emit d->synchronizerChanged( synchronizer );
}
inline KAbstractDocument::Private::~Private() { delete mSynchronizer; }



KAbstractDocument::KAbstractDocument()
 : d( new Private(this) )
{}

KAbstractDocumentSynchronizer *KAbstractDocument::synchronizer() const { return d->synchronizer(); }
void KAbstractDocument::setSynchronizer( KAbstractDocumentSynchronizer *synchronizer ) { d->setSynchronizer(synchronizer); }

bool KAbstractDocument::isModifiable() const { return false; }
bool KAbstractDocument::isReadOnly()   const { return true; }
void KAbstractDocument::setReadOnly( bool isReadOnly ) {}

bool KAbstractDocument::hasLocalChanges() const { return synchronizationStates().testFlag(LocalHasChanges) ; }

KAbstractDocument::~KAbstractDocument()
{
    delete d;
}
