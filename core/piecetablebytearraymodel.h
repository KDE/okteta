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

class PieceTableByteArrayModelPrivate;


/** 
  *@author Friedrich W. H. Kossebau
  */

class OKTETACORE_EXPORT PieceTableByteArrayModel : public AbstractByteArrayModel,
                                                   public Versionable,
                                                   public Bookmarkable,
                                                   public ChangesDescribable,
                                                   public ChangeHistory
{
  Q_OBJECT
  Q_INTERFACES(
    Okteta::Versionable
    Okteta::Bookmarkable
    Okteta::ChangesDescribable
    Okteta::ChangeHistory
  )

  friend class PieceTableByteArrayModelPrivate;

  public:
    explicit PieceTableByteArrayModel( const QByteArray& data, QObject* parent = 0 );
    explicit PieceTableByteArrayModel( int size = 0, Byte fillByte = '\0', QObject* parent = 0 );

    virtual ~PieceTableByteArrayModel();

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
    void setData( const QByteArray& data );

  Q_SIGNALS: // Versionable signals
    /*virtual*/ void revertedToVersionIndex( int versionIndex );
    /*virtual*/ void headVersionDescriptionChanged( const QString& versionDescription );
    /*virtual*/ void headVersionChanged( int newHeadVersionIndex );

  Q_SIGNALS: // Bookmarkable signals
    /*virtual*/ void bookmarksAdded( const QList<Okteta::Bookmark> &bookmarks );
    /*virtual*/ void bookmarksRemoved( const QList<Okteta::Bookmark> &bookmarks );
    /*virtual*/ void bookmarksModified( bool modified );
    /*virtual*/ void bookmarksModified( const QList<int>& indizes );

  Q_SIGNALS: // ChangeHistory signals
    /*virtual*/ void changesDone( const QList<Okteta::ByteArrayChange>& changes,
                                  int oldVersionIndex, int newVersionIndex );

  protected:
    PieceTableByteArrayModelPrivate* const d;
};

}

#endif
