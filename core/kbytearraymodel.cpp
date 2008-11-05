/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2003,2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kbytearraymodel.h"

// lib
#include "kbytearraymodel_p.h"
#include "arraychangemetricslist.h"


namespace KHECore
{

KByteArrayModel::KByteArrayModel( char *data, unsigned int size, int rawSize, bool keepMemory )
 : d( new KByteArrayModelPrivate(this,data,size,rawSize,keepMemory) )
{}

KByteArrayModel::KByteArrayModel( const char *data, unsigned int size )
 : d( new KByteArrayModelPrivate(this,data,size) )
{}

KByteArrayModel::KByteArrayModel( int size, int maxSize )
 : d( new KByteArrayModelPrivate(this,size,maxSize) )
{}


char KByteArrayModel::datum( unsigned int offset ) const { return d->datum(offset); }
int KByteArrayModel::size()                        const { return d->size(); }

bool KByteArrayModel::isReadOnly()   const { return d->isReadOnly(); }
bool KByteArrayModel::isModified()   const { return d->isModified(); }

void KByteArrayModel::setReadOnly( bool readOnly )       { d->setReadOnly( readOnly ); }
void KByteArrayModel::setModified( bool modified )       { d->setModified( modified ); }
void KByteArrayModel::setMaxSize( int maxSize )          { d->setMaxSize( maxSize ); }
void KByteArrayModel::setKeepsMemory( bool keepsMemory ) { d->setKeepsMemory( keepsMemory ); }
void KByteArrayModel::setAutoDelete( bool autoDelete )   { d->setAutoDelete( autoDelete ); }

void KByteArrayModel::setData( char *data, unsigned int size, int rawSize, bool keepMemory )
{ d->setData( data, size, rawSize, keepMemory ); }

char *KByteArrayModel::data()       const { return d->data(); }
int KByteArrayModel::maxSize()      const { return d->maxSize(); }
bool KByteArrayModel::keepsMemory() const { return d->keepsMemory(); }
bool KByteArrayModel::autoDelete()  const { return d->autoDelete(); }

void KByteArrayModel::signalContentsChanged( int start, int end )
{
    const int length = end - start + 1;
    emit contentsChanged( ArrayChangeMetricsList::oneReplacement(start,length,length) );
}


void KByteArrayModel::setDatum( unsigned int offset, const char datum )
{
    d->setDatum( offset, datum );
}

int KByteArrayModel::insert( int at, const char *data, int length )
{
    return d->insert( at, data, length );
}

int KByteArrayModel::remove( const Section &section )
{
    return d->remove( section );
}

unsigned int KByteArrayModel::replace( const Section &before, const char *after, unsigned int afterLength )
{
    return d->replace( before, after, afterLength );
}

bool KByteArrayModel::swap( int firstStart, const Section &secondSection )
{
    return d->swap( firstStart, secondSection );
}

int KByteArrayModel::fill( const char fillChar, unsigned int from, int length )
{
    return d->fill( fillChar, from, length );
}

int KByteArrayModel::indexOf( const char *searchString, int length, int from ) const
{
    return d->indexOf( searchString, length, from );
}

int KByteArrayModel::lastIndexOf( const char *searchString, int length, int from ) const
{
    return d->lastIndexOf( searchString, length, from );
}

void KByteArrayModel::addBookmarks( const QList<KHECore::Bookmark> &bookmarks )
{
    d->addBookmarks( bookmarks );
}

void KByteArrayModel::removeBookmarks( const QList<KHECore::Bookmark> &bookmarks )
{
    d->removeBookmarks( bookmarks );
}

void KByteArrayModel::removeAllBookmarks()
{
    d->removeAllBookmarks();
}

KHECore::BookmarkList KByteArrayModel::bookmarkList() const
{
    return d->bookmarkList();
}

KByteArrayModel::~KByteArrayModel()
{
    delete d;
}

}
