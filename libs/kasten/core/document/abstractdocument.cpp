/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractdocument.h"

// lib
#include "abstractmodelsynchronizer.h"


namespace Kasten
{

class AbstractDocument::Private
{
  public:
      explicit Private( AbstractDocument* parent );
      ~Private();
  public:
    void setId( const QString& id );
    QString id() const;
    void setSynchronizer( AbstractModelSynchronizer* synchronizer );
    void setLiveSynchronizer( AbstractModelSynchronizer* synchronizer );
    AbstractModelSynchronizer* synchronizer() const;
    AbstractModelSynchronizer* liveSynchronizer() const;
  protected:
    AbstractDocument* d;
    QString mId;
    AbstractModelSynchronizer* mSynchronizer; // TODO: should this be here, with public setters and getters?
    AbstractModelSynchronizer* mLiveSynchronizer; // TODO: should this be here, with public setters and getters?
};

inline AbstractDocument::Private::Private( AbstractDocument* parent )
: d( parent ), mSynchronizer( 0 ), mLiveSynchronizer( 0 ) {}
inline QString AbstractDocument::Private::id() const { return mId; }
inline void AbstractDocument::Private::setId( const QString& id ) { mId = id; }

inline AbstractModelSynchronizer* AbstractDocument::Private::synchronizer() const { return mSynchronizer; }
inline void AbstractDocument::Private::setSynchronizer( AbstractModelSynchronizer* synchronizer )
{
    // plugging the same more than once?
    if( mSynchronizer == synchronizer )
        return;

    delete mSynchronizer;
    mSynchronizer = synchronizer;
    emit d->synchronizerChanged( synchronizer );
}
inline AbstractModelSynchronizer* AbstractDocument::Private::liveSynchronizer() const { return mLiveSynchronizer; }
inline void AbstractDocument::Private::setLiveSynchronizer( AbstractModelSynchronizer* synchronizer )
{
    // plugging the same more than once?
    if( mLiveSynchronizer == synchronizer )
        return;

    delete mLiveSynchronizer;
    mLiveSynchronizer = synchronizer;
    emit d->liveSynchronizerChanged( synchronizer );
}
inline AbstractDocument::Private::~Private()
{
    delete mSynchronizer;
    delete mLiveSynchronizer;
}



AbstractDocument::AbstractDocument()
 : d( new Private(this) )
{}

QString AbstractDocument::id() const { return d->id(); }
AbstractModelSynchronizer* AbstractDocument::synchronizer() const { return d->synchronizer(); }
AbstractModelSynchronizer* AbstractDocument::liveSynchronizer() const { return d->liveSynchronizer(); }

void AbstractDocument::setId( const QString& id ) { d->setId(id); }
void AbstractDocument::setSynchronizer( AbstractModelSynchronizer* synchronizer ) { d->setSynchronizer(synchronizer); }
void AbstractDocument::setLiveSynchronizer( AbstractModelSynchronizer* synchronizer ) { d->setLiveSynchronizer(synchronizer); }

bool AbstractDocument::hasLocalChanges() const { return synchronizationStates().testFlag(LocalHasChanges) ; }

AbstractDocument::~AbstractDocument()
{
    delete d;
}

}
