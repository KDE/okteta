/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractloadjob.h"

class AbstractLoadJob::Private
{
  public:
    Private();

  public:
    KAbstractDocument *document() const;
    void setDocument( KAbstractDocument *document );

  protected:
    KAbstractDocument *mDocument;
};

AbstractLoadJob::Private::Private()
 : mDocument( 0 )
{}


inline KAbstractDocument *AbstractLoadJob::Private::document() const { return mDocument; }
inline void AbstractLoadJob::Private::setDocument( KAbstractDocument *document ) { mDocument = document; }


AbstractLoadJob::AbstractLoadJob()
 : d( new Private() )
{}

KAbstractDocument *AbstractLoadJob::document() const { return d->document(); }

void AbstractLoadJob::setDocument( KAbstractDocument *document )
{
    if( document )
    {
        d->setDocument( document );
        emit documentLoaded( document );
    }

    emitResult();
}

AbstractLoadJob::~AbstractLoadJob()
{
    delete d;
}
