/***************************************************************************
                          kbytearraymodel.h  -  description
                             -------------------
    begin                : Mit Jun 03 2003
    copyright            : (C) 2003,2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_CORE_KBYTEARRAYMODEL_H
#define KHE_CORE_KBYTEARRAYMODEL_H


// lib
#include "kabstractbytearraymodel.h"
#include "kibookmarks.h"


namespace KHECore
{

class KByteArrayModelPrivate;

/** 
  *@author Friedrich W. H. Kossebau
  */

class KHECORE_EXPORT KByteArrayModel : public KAbstractByteArrayModel, public KDE::If::Bookmarks
{
    Q_OBJECT
    Q_INTERFACES( KDE::If::Bookmarks )

    friend class KByteArrayModelPrivate;

  public:
    KByteArrayModel( char *data, unsigned int size, int rawSize = -1, bool keepMemory = true );
    KByteArrayModel( const char *data, unsigned int size );
    explicit KByteArrayModel( int size=0, int maxSize = -1 );
    virtual ~KByteArrayModel();

  public: // KAbstractByteArrayModel API
    virtual char datum( unsigned int offset ) const;
    virtual int size() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

    virtual int insert( int at, const char *data, int length );
    virtual int remove( const KSection &section );
    virtual unsigned int replace( const KSection &before, const char *after, unsigned int afterLength );
    virtual int move( int to, const KSection &fromSection );
    virtual int fill( const char fillChar, unsigned int from = 0, int length = -1 );
    virtual void setDatum( unsigned int offset, const char datum );

    virtual void setModified( bool modified = true );
    virtual void setReadOnly( bool isReadOnly = true );

    virtual int indexOf( const char *searchString, int length, int from  = 0 ) const;
    virtual int lastIndexOf( const char *searchString, int length, int from = -1 ) const;

  public: // KDE::If::Bookmarks API
    virtual void addBookmarks( const QList<KHECore::KBookmark> &bookmarks );
    virtual void removeBookmarks( const QList<KHECore::KBookmark> &bookmarks );
    virtual void removeAllBookmarks();

    virtual KHECore::KBookmarkList bookmarkList() const;

  Q_SIGNALS: // KDE::If::Bookmarks API
    virtual void bookmarksAdded( const QList<KHECore::KBookmark> &bookmarks );
    virtual void bookmarksRemoved( const QList<KHECore::KBookmark> &bookmarks );
    virtual void bookmarksModified( bool modified );

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
    KByteArrayModelPrivate * const d;
};

}

#endif
