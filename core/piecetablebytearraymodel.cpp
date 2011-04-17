/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
// TODO: a wrapper to a raw data, plus a function to dump the current version to the original data

PieceTableByteArrayModel::PieceTableByteArrayModel( const QByteArray& data, QObject* parent )
  : AbstractByteArrayModel( parent ),
    d( new PieceTableByteArrayModelPrivate(this,data) )
{}

PieceTableByteArrayModel::PieceTableByteArrayModel( int size, Byte fillByte, QObject* parent )
  : AbstractByteArrayModel( parent ),
    d( new PieceTableByteArrayModelPrivate(this,size,fillByte) )
{}


Byte PieceTableByteArrayModel::byte( Address offset ) const { return d->byte(offset); }
Size PieceTableByteArrayModel::size()                 const { return d->size(); }

bool PieceTableByteArrayModel::isReadOnly()   const { return d->isReadOnly(); }
bool PieceTableByteArrayModel::isModified()   const { return d->isModified(); }

void PieceTableByteArrayModel::setReadOnly( bool readOnly )       { d->setReadOnly( readOnly ); }
void PieceTableByteArrayModel::setModified( bool modified )       { d->setModified( modified ); }

void PieceTableByteArrayModel::setData( const QByteArray& data )
{
    d->setData( data );
}

void PieceTableByteArrayModel::setByte( Address offset, Byte byte )
{
    d->setByte( offset, byte );
}

Size PieceTableByteArrayModel::insert( Address offset, const Byte* insertData, int insertLength )
{
    return d->insert( offset, insertData, insertLength );
}

Size PieceTableByteArrayModel::remove( const AddressRange& removeRange )
{
    return d->remove( removeRange );
}

Size PieceTableByteArrayModel::replace( const AddressRange& removeRange, const Byte* insertData, int insertLength )
{
    return d->replace( removeRange, insertData, insertLength );
}

bool PieceTableByteArrayModel::swap( Address firstStart, const AddressRange& secondRange )
{
    return d->swap( firstStart, secondRange );
}

Size PieceTableByteArrayModel::fill( Byte fillByte, Address offset, Size fillLength )
{
    return d->fill( fillByte, offset, fillLength );
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
    d->doChanges( changes, oldVersionIndex, newVersionIndex );
}


PieceTableByteArrayModel::~PieceTableByteArrayModel()
{
    delete d;
}

}
