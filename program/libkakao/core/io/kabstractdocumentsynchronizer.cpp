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
