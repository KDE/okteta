/*
    This file is part of the Okteta Core library, part of the KDE project.

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
    Q_INTERFACES( Okteta::Bookmarkable )

    friend class ByteArrayModelPrivate;

  public:
    ByteArrayModel( char *data, unsigned int size, int rawSize = -1, bool keepMemory = true );
    ByteArrayModel( const char *data, unsigned int size );
    explicit ByteArrayModel( int size=0, int maxSize = -1 );
    virtual ~ByteArrayModel();

  public: // AbstractByteArrayModel API
    virtual char datum( unsigned int offset ) const;
    virtual int size() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

    virtual int insert( int at, const char *data, int length );
    virtual int remove( const KDE::Section& section );
    virtual unsigned int replace( const KDE::Section& before, const char* after, unsigned int afterLength );
    virtual bool swap( int firstStart, const KDE::Section& secondSection );
    virtual int fill( const char fillChar, unsigned int from = 0, int length = -1 );
    virtual void setDatum( unsigned int offset, const char datum );

    virtual void setModified( bool modified = true );
    virtual void setReadOnly( bool isReadOnly = true );

    virtual int indexOf( const char *searchString, int length, int from  = 0 ) const;
    virtual int lastIndexOf( const char *searchString, int length, int from = -1 ) const;

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
    virtual void bookmarksAdded( const QList<Okteta::Bookmark> &bookmarks );
    virtual void bookmarksRemoved( const QList<Okteta::Bookmark> &bookmarks );
    virtual void bookmarksModified( bool modified );
    virtual void bookmarksModified( const QList<int>& indizes );

  public:
    void setMaxSize( int MS );
    /** sets whether the memory given by setData or in the constructor should be kept on resize
      */
    void setKeepsMemory( bool keepMemory = true );
    void setAutoDelete( bool autoDelete = true );
    void setData( char *data, unsigned int size, int rawSize = -1, bool keepMemory = true );
    void signalContentsChanged( int i1, int i2 );

  public:
    char *data() const;
    int maxSize() const;
    /** returns whether the memory of the byte array is kept on resize */
    bool keepsMemory() const;
    bool autoDelete() const;

  protected:
    ByteArrayModelPrivate * const d;
};

}

#endif
