/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_FIXEDSIZEBYTEARRAYMODEL_H
#define OKTETA_FIXEDSIZEBYTEARRAYMODEL_H

// lib
#include "abstractbytearraymodel.h"


namespace Okteta
{

/** base class for all mData buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *@author Friedrich W. H. Kossebau
  */

class FixedSizeByteArrayModel : public AbstractByteArrayModel
{
  public:
    /** creates a readonly buffer around the given data */
    FixedSizeByteArrayModel( Byte* data, int size, Byte fillUpChar = '\0', QObject* parent = 0 );
    /** creates a writeable buffer which is deleted at the end */
    explicit FixedSizeByteArrayModel( int size, Byte fillUpChar = '\0', QObject* parent = 0 );

    virtual ~FixedSizeByteArrayModel();

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
    virtual void setReadOnly( bool readOnly = true );

  public:
    int compare( const AbstractByteArrayModel& other, const AddressRange& otherRange, Address offset = 0 );
    int compare( const AbstractByteArrayModel& other, Address otherOffset, Size otherLength, Address offset = 0 );
    int compare( const AbstractByteArrayModel& other );

  public:
    Byte* rawData() const;

  protected:
    void reset( unsigned int pos, unsigned int length );

  protected:
    /** */
    Byte* mData;
    /***/
    int mSize;
    /** */
    Byte mFillUpByte;
    /**  */
    bool mReadOnly :1;
    /** */
    bool mModified :1;
    /** */
    bool mAutoDelete :1;
};


inline Byte FixedSizeByteArrayModel::byte( Address offset ) const { return mData[offset]; }
inline Size FixedSizeByteArrayModel::size() const  { return mSize; }

inline bool FixedSizeByteArrayModel::isReadOnly()   const { return mReadOnly; }
inline bool FixedSizeByteArrayModel::isModified()   const { return mModified; }

inline void FixedSizeByteArrayModel::setReadOnly( bool readOnly )  { mReadOnly = readOnly; }
inline void FixedSizeByteArrayModel::setModified( bool modified )   { mModified = modified; }

inline int FixedSizeByteArrayModel::compare( const AbstractByteArrayModel& other )
{ return compare( other, AddressRange::fromWidth(0,other.size()), 0 ); }

inline int FixedSizeByteArrayModel::compare( const AbstractByteArrayModel& other, Address otherOffset, Size otherLength, Address offset )
{ return compare( other, AddressRange::fromWidth(otherOffset,otherLength), offset ); }

inline Byte* FixedSizeByteArrayModel::rawData() const { return mData; }

}

#endif
