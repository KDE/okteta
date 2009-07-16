/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#include "piecetablebytearraymodel.h"
#include "piecetablebytearraymodel_p.h"


namespace Okteta
{

PieceTableByteArrayModel::PieceTableByteArrayModel( const char *data, unsigned int size,
                                                      bool careForMemory )
 : d( new Private(this,data,size,careForMemory) )
{}

PieceTableByteArrayModel::PieceTableByteArrayModel( unsigned int size, char fillByte )
 : d( new Private(this,size,fillByte) )
{}

// PieceTableByteArrayModel::PieceTableByteArrayModel( int size, int maxSize )
//  : d( new Private(this,size,maxSize) )
// {}


char PieceTableByteArrayModel::datum( unsigned int offset ) const { return d->datum(offset); }
int PieceTableByteArrayModel::size()                        const { return d->size(); }

bool PieceTableByteArrayModel::isReadOnly()   const { return d->isReadOnly(); }
bool PieceTableByteArrayModel::isModified()   const { return d->isModified(); }

void PieceTableByteArrayModel::setReadOnly( bool readOnly )       { d->setReadOnly( readOnly ); }
void PieceTableByteArrayModel::setModified( bool modified )       { d->setModified( modified ); }
// void PieceTableByteArrayModel::setMaxSize( int maxSize )          { d->setMaxSize( maxSize ); }
// void PieceTableByteArrayModel::setKeepsMemory( bool keepsMemory ) { d->setKeepsMemory( keepsMemory ); }
// void PieceTableByteArrayModel::setAutoDelete( bool autoDelete )   { d->setAutoDelete( autoDelete ); }

void PieceTableByteArrayModel::setData( const char *data, unsigned int size, bool careForMemory )
{ d->setData( data, size, careForMemory ); }

// char *PieceTableByteArrayModel::data()       const { return d->data(); }
// int PieceTableByteArrayModel::maxSize()      const { return d->maxSize(); }
// bool PieceTableByteArrayModel::keepsMemory() const { return d->keepsMemory(); }
// bool PieceTableByteArrayModel::autoDelete()  const { return d->autoDelete(); }


void PieceTableByteArrayModel::setDatum( unsigned int offset, const char datum )
{
    d->setDatum( offset, datum );
}

int PieceTableByteArrayModel::insert( int at, const char *data, int length )
{
    return d->insert( at, data, length );
}

int PieceTableByteArrayModel::remove( const Section &section )
{
    return d->remove( section );
}

unsigned int PieceTableByteArrayModel::replace( const Section &before, const char *after, unsigned int afterLength )
{
    return d->replace( before, after, afterLength );
}

bool PieceTableByteArrayModel::swap( int firstStart, const Section &secondSection )
{
    return d->swap( firstStart, secondSection );
}

int PieceTableByteArrayModel::fill( const char fillChar, unsigned int from, int length )
{
    return d->fill( fillChar, from, length );
}

// int PieceTableByteArrayModel::indexOf( const char *searchString, int length, int from ) const
// {
//     return d->indexOf( searchString, length, from );
// }
// 
// int PieceTableByteArrayModel::lastIndexOf( const char *searchString, int length, int from ) const
// {
//     return d->lastIndexOf( searchString, length, from );
// }

int PieceTableByteArrayModel::versionIndex() const { return d->versionIndex(); }
int PieceTableByteArrayModel::versionCount() const { return d->versionCount(); }
QString PieceTableByteArrayModel::versionDescription( int versionIndex ) const { return d->versionDescription(versionIndex); }

void PieceTableByteArrayModel::revertToVersionByIndex( int versionIndex )
{ d->revertToVersionByIndex( versionIndex ); }

void PieceTableByteArrayModel::addBookmarks( const QList<Okteta::Bookmark> &bookmarks )
{
    d->addBookmarks( bookmarks );
}

void PieceTableByteArrayModel::removeBookmarks( const QList<Okteta::Bookmark> &bookmarks )
{
    d->removeBookmarks( bookmarks );
}

void PieceTableByteArrayModel::removeAllBookmarks()
{
    d->removeAllBookmarks();
}

void PieceTableByteArrayModel::setBookmark( unsigned int index, const Okteta::Bookmark& bookmark )
{
    d->setBookmark( index, bookmark );
}

Okteta::BookmarksConstIterator PieceTableByteArrayModel::createBookmarksConstIterator() const
{
    return d->createBookmarksConstIterator();
}

const Okteta::Bookmark& PieceTableByteArrayModel::bookmarkAt( unsigned int index ) const
{
    return d->bookmarkAt( index );
}

const Okteta::Bookmark& PieceTableByteArrayModel::bookmarkFor( int offset ) const
{
    return d->bookmarkFor( offset );
}

bool PieceTableByteArrayModel::containsBookmarkFor( int offset ) const
{
    return d->containsBookmarkFor( offset );
}

unsigned int PieceTableByteArrayModel::bookmarksCount() const
{
    return d->bookmarksCount();
}

void PieceTableByteArrayModel::openGroupedChange( const QString &description )
{
    d->openGroupedChange( description );
}

void PieceTableByteArrayModel::cancelGroupedChange()
{
    d->cancelGroupedChange();
}

void PieceTableByteArrayModel::closeGroupedChange( const QString &description )
{
    d->closeGroupedChange( description );
}

QList<ByteArrayChange> PieceTableByteArrayModel::changes( int firstVersionIndex, int lastVersionIndex ) const
{
    return d->changes( firstVersionIndex, lastVersionIndex );
}

QByteArray PieceTableByteArrayModel::initialData() const
{
    return d->initialData();
}

void PieceTableByteArrayModel::doChanges( const QList<Okteta::ByteArrayChange>& changes,
                                           int oldVersionIndex, int newVersionIndex )
{
    return d->doChanges( changes, oldVersionIndex, newVersionIndex );
}


PieceTableByteArrayModel::~PieceTableByteArrayModel()
{
    delete d;
}

}
