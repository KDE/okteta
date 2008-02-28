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
