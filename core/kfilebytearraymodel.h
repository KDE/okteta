/***************************************************************************
                          kfilebytearraymodel.h  -  description
                             -------------------
    begin                : Mit Jun 02 2003
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


#ifndef KHE_CORE_KFILEBYTEARRAYMODEL_H
#define KHE_CORE_KFILEBYTEARRAYMODEL_H


// lib specific
#include "kabstractbytearraymodel.h"


namespace KHECore {

class KFileByteArrayModelPrivate;

/**
  *@author Friedrich W. H. Kossebau
  */

class KHECORE_EXPORT KFileByteArrayModel : public KAbstractByteArrayModel
{
    friend class KFileByteArrayModelPrivate;

  public:
    /** default is only 50*4k = 200k memory image */
    KFileByteArrayModel( int pageNumber = 50, int pageSize = 4096 );
    virtual ~KFileByteArrayModel();

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

  public:
    void setReadOnly( bool readOnly = true );
    bool isOpen() const;
    bool open( const QString &filename );
    bool close();

  protected:
    KFileByteArrayModelPrivate * const d;
};

}

#endif
