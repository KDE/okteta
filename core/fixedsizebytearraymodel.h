/*
    This file is part of the Okteta Core library, part of the KDE project.

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
    FixedSizeByteArrayModel( char* data, unsigned int size, char fillUpChar = '\0' );
    /** creates a writeable buffer which is deleted at the end */
    explicit FixedSizeByteArrayModel( unsigned int size, char fillUpChar = '\0' );

    virtual ~FixedSizeByteArrayModel();

  public: // AbstractByteArrayModel API
    virtual char datum( unsigned int offset ) const;
    virtual int size() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

    virtual int insert( int offset, const char* insertData, int insertLength );
    virtual int remove( const KDE::Section& removeSection );
    virtual unsigned int replace( const KDE::Section& removeSection, const char* insertData, unsigned int insertLength );
    virtual bool swap( int firstStart, const KDE::Section& secondSection );
    virtual int fill( const char fillChar, unsigned int offset = 0, int fillLength = -1 );
    virtual void setDatum( unsigned int offset, const char datum );

    virtual void setModified( bool modified = true );
    virtual void setReadOnly( bool readOnly = true );

  public:
    int compare( const AbstractByteArrayModel& other, const KDE::Section& range, unsigned int pos = 0 );
    int compare( const AbstractByteArrayModel& other, int otherPos, int length, unsigned int pos = 0 );
    int compare( const AbstractByteArrayModel& other );

  public:
    char* rawData() const;

  protected:
    void reset( unsigned int pos, unsigned int length );

  protected:
    /** */
    char* mData;
    /***/
    unsigned int mSize;
    /** */
    char mFillUpChar;
    /**  */
    bool mReadOnly :1;
    /** */
    bool mModified :1;
    /** */
    bool mAutoDelete :1;
};


inline char FixedSizeByteArrayModel::datum( unsigned int offset ) const { return mData[offset]; }
inline int FixedSizeByteArrayModel::size() const  { return mSize; }

inline bool FixedSizeByteArrayModel::isReadOnly()   const { return mReadOnly; }
inline bool FixedSizeByteArrayModel::isModified()   const { return mModified; }

inline void FixedSizeByteArrayModel::setReadOnly( bool readOnly )  { mReadOnly = readOnly; }
inline void FixedSizeByteArrayModel::setModified( bool modified )   { mModified = modified; }

inline int FixedSizeByteArrayModel::compare( const AbstractByteArrayModel& other )
{ return compare( other, KDE::Section(0,other.size()-1),0 ); }

inline int FixedSizeByteArrayModel::compare( const AbstractByteArrayModel& other, int otherPos, int length, unsigned int pos )
{ return compare( other, KDE::Section::fromWidth(otherPos,length),pos ); }

inline char* FixedSizeByteArrayModel::rawData() const { return mData; }

}

#endif
