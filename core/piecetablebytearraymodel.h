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

#ifndef OKTETA_PIECETABLEBYTEARRAYMODEL_H
#define OKTETA_PIECETABLEBYTEARRAYMODEL_H

// lib
#include "abstractbytearraymodel.h"
#include "versionable.h"
#include "bookmarkable.h"
#include "changesdescribable.h"
#include "changehistory.h"


namespace Okteta
{

/** 
  *@author Friedrich W. H. Kossebau
  */

class OKTETACORE_EXPORT PieceTableByteArrayModel : public AbstractByteArrayModel, public Versionable, public Bookmarkable, public ChangesDescribable, public ChangeHistory
{
    Q_OBJECT
    Q_INTERFACES( Okteta::Versionable Okteta::Bookmarkable Okteta::ChangesDescribable Okteta::ChangeHistory )

    class Private;
    friend class Private;

  public:
//     PieceTableByteArrayModel( const char *data, unsigned int size );
//     PieceTableByteArrayModel( char *D, unsigned int size );
    /**
     * makes a deep copy if careForMemory is false
     * @param data
     * @param careForMemory 
     */
    PieceTableByteArrayModel( const char *data, unsigned int size, bool careForMemory = true );
//     explicit PieceTableByteArrayModel( const QByteArray &data );
    explicit PieceTableByteArrayModel( unsigned int size = 0, char fillByte = '\0' );
//     explicit PieceTableByteArrayModel( int size = 0careForMemory, int maxSize = -1 );
    virtual ~PieceTableByteArrayModel();

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

//     virtual int indexOf( const char *searchString, int length, int from  = 0 ) const;
//     virtual int lastIndexOf( const char *searchString, int length, int from = -1 ) const;

  public: // Versionable API
    virtual int versionIndex() const;
    virtual int versionCount() const;
    virtual QString versionDescription( int versionIndex ) const;

  public: // set/action
    virtual void revertToVersionByIndex( int versionIndex );

  public: // Bookmarkable API
    virtual void addBookmarks( const QList<Okteta::Bookmark> &bookmarks );
    virtual void removeBookmarks( const QList<Okteta::Bookmark> &bookmarks );
    virtual void removeAllBookmarks();
    virtual void setBookmark( unsigned int index, const Okteta::Bookmark& bookmark );

    virtual Okteta::BookmarksConstIterator createBookmarksConstIterator() const;
    virtual const Okteta::Bookmark& bookmarkAt( unsigned int index ) const;
    virtual const Okteta::Bookmark& bookmarkFor( int offset ) const;
    virtual bool containsBookmarkFor( int offset ) const;
    virtual unsigned int bookmarksCount() const;

  public: // ChangesDescribable API
    virtual void openGroupedChange( const QString &description );
    virtual void cancelGroupedChange();
    virtual void closeGroupedChange( const QString &description );

  public: // ChangeHistory API
    virtual QList<ByteArrayChange> changes( int firstVersionIndex, int lastVersionIndex ) const;
    virtual QByteArray initialData() const;
    virtual void doChanges( const QList<Okteta::ByteArrayChange>& changes,
                            int oldVersionIndex, int newVersionIndex );

  public:
//     void setMaxSize( int MS );
    /** sets whether the memory given by setData or in the constructor should be kept on resize
      */
//     void setKeepsMemory( bool keepMemory = true );
//     void setAutoDelete( bool autoDelete = true );
    void setData( const char *data, unsigned int size, bool careForMemory = true );

  public:
//     char *data() const;
//     int maxSize() const;
    /** returns whether the memory of the byte array is kept on resize */
//     bool keepsMemory() const;
//     bool autoDelete() const;

  Q_SIGNALS: // Versionable signals
    virtual void revertedToVersionIndex( int versionIndex );
    virtual void headVersionDescriptionChanged( const QString &versionDescription );
    virtual void headVersionChanged( int newHeadVersionIndex );

  Q_SIGNALS: // Bookmarkable signals
    virtual void bookmarksAdded( const QList<Okteta::Bookmark> &bookmarks );
    virtual void bookmarksRemoved( const QList<Okteta::Bookmark> &bookmarks );
    virtual void bookmarksModified( bool modified );
    virtual void bookmarksModified( const QList<int>& indizes );

  Q_SIGNALS: // ChangeHistory signals
    virtual void changesDone( const QList<Okteta::ByteArrayChange>& changes,
                              int oldVersionIndex, int newVersionIndex );

  protected:
    class Private * const d;
};

}

#endif
