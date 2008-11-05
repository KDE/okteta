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

#ifndef KHE_CORE_KFILEBYTEARRAYMODEL_H
#define KHE_CORE_KFILEBYTEARRAYMODEL_H


// lib
#include "abstractbytearraymodel.h"


namespace KHECore {

class KFileByteArrayModelPrivate;

/**
  *@author Friedrich W. H. Kossebau
  */

class OKTETACORE_EXPORT KFileByteArrayModel : public AbstractByteArrayModel
{
    friend class KFileByteArrayModelPrivate;

  public:
    /** default is only 50*4k = 200k memory image */
    explicit KFileByteArrayModel( int pageNumber = 50, int pageSize = 4096 );
    virtual ~KFileByteArrayModel();

  public: // AbstractByteArrayModel API
    virtual char datum( unsigned int offset ) const;
    virtual int size() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

    virtual int insert( int at, const char *data, int length );
    virtual int remove( const Section &section );
    virtual unsigned int replace( const Section &before, const char *after, unsigned int afterLength );
    virtual bool swap( int firstStart, const Section &secondSection );
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
