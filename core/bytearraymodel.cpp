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

#include "bytearraymodel.h"

// lib
#include "bytearraymodel_p.h"
#include "arraychangemetricslist.h"


namespace Okteta
{

ByteArrayModel::ByteArrayModel( char *data, unsigned int size, int rawSize, bool keepMemory )
 : d( new ByteArrayModelPrivate(this,data,size,rawSize,keepMemory) )
{}

ByteArrayModel::ByteArrayModel( const char *data, unsigned int size )
 : d( new ByteArrayModelPrivate(this,data,size) )
{}

ByteArrayModel::ByteArrayModel( int size, int maxSize )
 : d( new ByteArrayModelPrivate(this,size,maxSize) )
{}


char ByteArrayModel::datum( unsigned int offset ) const { return d->datum(offset); }
int ByteArrayModel::size()                        const { return d->size(); }

bool ByteArrayModel::isReadOnly()   const { return d->isReadOnly(); }
bool ByteArrayModel::isModified()   const { return d->isModified(); }

void ByteArrayModel::setReadOnly( bool readOnly )       { d->setReadOnly( readOnly ); }
void ByteArrayModel::setModified( bool modified )       { d->setModified( modified ); }
void ByteArrayModel::setMaxSize( int maxSize )          { d->setMaxSize( maxSize ); }
void ByteArrayModel::setKeepsMemory( bool keepsMemory ) { d->setKeepsMemory( keepsMemory ); }
void ByteArrayModel::setAutoDelete( bool autoDelete )   { d->setAutoDelete( autoDelete ); }

void ByteArrayModel::setData( char *data, unsigned int size, int rawSize, bool keepMemory )
{ d->setData( data, size, rawSize, keepMemory ); }

char *ByteArrayModel::data()       const { return d->data(); }
int ByteArrayModel::maxSize()      const { return d->maxSize(); }
bool ByteArrayModel::keepsMemory() const { return d->keepsMemory(); }
bool ByteArrayModel::autoDelete()  const { return d->autoDelete(); }

void ByteArrayModel::signalContentsChanged( int start, int end )
{
    const int length = end - start + 1;
    emit contentsChanged( ArrayChangeMetricsList::oneReplacement(start,length,length) );
}


void ByteArrayModel::setDatum( unsigned int offset, const char datum )
{
    d->setDatum( offset, datum );
}

int ByteArrayModel::insert( int at, const char *data, int length )
{
    return d->insert( at, data, length );
}

int ByteArrayModel::remove( const Section &section )
{
    return d->remove( section );
}

unsigned int ByteArrayModel::replace( const Section &before, const char *after, unsigned int afterLength )
{
    return d->replace( before, after, afterLength );
}

bool ByteArrayModel::swap( int firstStart, const Section &secondSection )
{
    return d->swap( firstStart, secondSection );
}

int ByteArrayModel::fill( const char fillChar, unsigned int from, int length )
{
    return d->fill( fillChar, from, length );
}

int ByteArrayModel::indexOf( const char *searchString, int length, int from ) const
{
    return d->indexOf( searchString, length, from );
}

int ByteArrayModel::lastIndexOf( const char *searchString, int length, int from ) const
{
    return d->lastIndexOf( searchString, length, from );
}

void ByteArrayModel::addBookmarks( const QList<Okteta::Bookmark> &bookmarks )
{
    d->addBookmarks( bookmarks );
}

void ByteArrayModel::removeBookmarks( const QList<Okteta::Bookmark> &bookmarks )
{
    d->removeBookmarks( bookmarks );
}

void ByteArrayModel::removeAllBookmarks()
{
    d->removeAllBookmarks();
}

void ByteArrayModel::setBookmark( unsigned int index, const Okteta::Bookmark& bookmark )
{
    d->setBookmark( index, bookmark );
}

Okteta::BookmarksConstIterator ByteArrayModel::createBookmarksConstIterator() const
{
    return d->createBookmarksConstIterator();
}

const Okteta::Bookmark& ByteArrayModel::bookmarkFor( int offset ) const
{
    return d->bookmarkFor( offset );
}

const Okteta::Bookmark& ByteArrayModel::bookmarkAt( unsigned int index ) const
{
    return d->bookmarkAt( index );
}

bool ByteArrayModel::containsBookmarkFor( int offset ) const
{
    return d->containsBookmarkFor( offset );
}

unsigned int ByteArrayModel::bookmarksCount() const
{
    return d->bookmarksCount();
}

ByteArrayModel::~ByteArrayModel()
{
    delete d;
}

}
