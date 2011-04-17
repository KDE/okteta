/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2003,2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_BYTEARRAYMODEL_H
#define OKTETA_BYTEARRAYMODEL_H

// lib
#include "abstractbytearraymodel.h"
#include "bookmarkable.h"
#include "bookmarksconstiterator.h"


namespace Okteta
{

class ByteArrayModelPrivate;

/** 
  *@author Friedrich W. H. Kossebau
  */

class OKTETACORE_EXPORT ByteArrayModel : public AbstractByteArrayModel,
                                         public Bookmarkable
{
  Q_OBJECT
  Q_INTERFACES(
    Okteta::Bookmarkable
  )

  friend class ByteArrayModelPrivate;

  public:
    ByteArrayModel( Byte* data, int size, int rawSize = -1, bool keepsMemory = true, QObject* parent = 0 );
    ByteArrayModel( const Byte* data, int size, QObject* parent = 0 );
    explicit ByteArrayModel( int size = 0, int maxSize = -1, QObject* parent = 0 );

    virtual ~ByteArrayModel();

  public: // AbstractByteArrayModel API
    virtual Byte byte( Address offset ) const;
    virtual Size size() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

    virtual Size insert( Address offset, const Byte* insertData, int insertLength );
    virtual Size remove( const AddressRange& removeRange );
    virtual Size replace( const AddressRange& removeRange, const Byte* insertData, int insertLength );
    virtual bool swap( Address firstStart, const AddressRange& secondRange );
    virtual Size fill( Byte fillByte, Address offset = 0, Size fillLength = -1 );
    virtual void setByte( Address offset, Byte byte );

    virtual void setModified( bool modified = true );
    virtual void setReadOnly( bool isReadOnly = true );

  public: // Okteta::Bookmarkable API
    virtual void addBookmarks( const QList<Okteta::Bookmark> &bookmarks );
    virtual void removeBookmarks( const QList<Okteta::Bookmark> &bookmarks );
    virtual void removeAllBookmarks();
    virtual void setBookmark( unsigned int index, const Okteta::Bookmark& bookmark );

    virtual Okteta::BookmarksConstIterator createBookmarksConstIterator() const;
    virtual const Okteta::Bookmark& bookmarkAt( unsigned int index ) const;
    virtual const Okteta::Bookmark& bookmarkFor( int offset ) const;
    virtual bool containsBookmarkFor( int offset ) const;
    virtual unsigned int bookmarksCount() const;

  Q_SIGNALS: // Okteta::Bookmarkable API
    /*virtual*/ void bookmarksAdded( const QList<Okteta::Bookmark> &bookmarks );
    /*virtual*/ void bookmarksRemoved( const QList<Okteta::Bookmark> &bookmarks );
    /*virtual*/ void bookmarksModified( bool modified );
    /*virtual*/ void bookmarksModified( const QList<int>& indizes );

  public:
    void setMaxSize( int maxSize );
    /** sets whether the memory given by setData or in the constructor should be kept on resize
      */
    void setKeepsMemory( bool keepsMemory = true );
    void setAutoDelete( bool autoDelete = true );
    void setData( Byte* data, int size, int rawSize = -1, bool keepsMemory = true );
    void signalContentsChanged( int i1, int i2 );

  public:
    Byte* data() const;
    int maxSize() const;
    /** returns whether the memory of the byte array is kept on resize */
    bool keepsMemory() const;
    bool autoDelete() const;

  protected:
    ByteArrayModelPrivate* const d;
};

}

#endif
