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

#ifndef FIXEDSIZEBYTEARRAYMODEL_H
#define FIXEDSIZEBYTEARRAYMODEL_H

// lib
#include "abstractbytearraymodel.h"


namespace Okteta
{

/** base class for all Data buffers that are used to display
  * TODO: think about a way to inform KHexEdit that there has been
  * a change in the buffer outside. what kind of changes are possible?
  *@author Friedrich W. H. Kossebau
  */

class FixedSizeByteArrayModel : public AbstractByteArrayModel
{
  public:
    /** creates a readonly buffer around the given data */
    FixedSizeByteArrayModel( char *D, unsigned int S, char FUC = '\0' );
    /** creates a writeable buffer which is deleted at the end */
    explicit FixedSizeByteArrayModel( unsigned int S, char FUC = '\0' );
    virtual ~FixedSizeByteArrayModel();

  public: // AbstractByteArrayModel API
    virtual bool prepareRange( const Section &Range ) const;
    virtual const char *dataSet( const Section &S ) const;
    virtual char datum( unsigned int Offset ) const;
    virtual int size() const;
    virtual bool isReadOnly() const;
    virtual bool isModified() const;

    virtual int insert( int Pos, const char*, int Length );
    virtual int remove( const Section &Remove );
    virtual unsigned int replace( const Section &Remove, const char*, unsigned int InputLength );
    virtual bool swap( int firstStart, const Section &secondSection );
    virtual int fill( const char FillChar, unsigned int Pos = 0, int Length = -1 );
    virtual void setDatum( unsigned int Offset, const char Char );

    virtual void setModified( bool M = true );
    virtual void setReadOnly( bool RO = true );

  public:
    int compare( const AbstractByteArrayModel &Other, const Section &Range, unsigned int Pos = 0 );
    int compare( const AbstractByteArrayModel &Other, int OtherPos, int Length, unsigned int Pos = 0 );
    int compare( const AbstractByteArrayModel &Other );

  public:
    char *rawData() const;

  protected:
    void reset( unsigned int Pos, unsigned int Length );

  protected:
    /** */
    char *Data;
    /***/
    unsigned int Size;
    /** */
    char FillUpChar;
    /**  */
    bool ReadOnly:1;
    /** */
    bool Modified:1;
    /** */
    bool AutoDelete:1;
};


inline bool FixedSizeByteArrayModel::prepareRange( const Section &) const { return true; }
inline const char *FixedSizeByteArrayModel::dataSet( const Section &S ) const { return &Data[S.start()]; }

inline char FixedSizeByteArrayModel::datum( unsigned int Offset ) const { return Data[Offset]; }
inline int FixedSizeByteArrayModel::size() const  { return Size; }

inline bool FixedSizeByteArrayModel::isReadOnly()   const { return ReadOnly; }
inline bool FixedSizeByteArrayModel::isModified()   const { return Modified; }

inline void FixedSizeByteArrayModel::setReadOnly( bool RO )  { ReadOnly = RO; }
inline void FixedSizeByteArrayModel::setModified( bool M )   { Modified = M; }

inline int FixedSizeByteArrayModel::compare( const AbstractByteArrayModel &Other )
{ return compare( Other, Section(0,Other.size()-1),0 ); }

inline int FixedSizeByteArrayModel::compare( const AbstractByteArrayModel &Other, int OtherPos, int Length, unsigned int Pos )
{ return compare( Other, Section::fromWidth(OtherPos,Length),Pos ); }

inline char *FixedSizeByteArrayModel::rawData() const { return Data; }

}

#endif
