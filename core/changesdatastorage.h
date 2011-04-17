/*
    This file is part of the Okteta Core library, made within the KDE community.

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

#ifndef OKTETA_CHANGESDATASTORAGE_H
#define OKTETA_CHANGESDATASTORAGE_H

// Qt
#include <QtCore/QByteArray>

namespace Okteta
{

class ChangesDataStorage
{
  public:
    ChangesDataStorage();

  public: // set/action
    void append( int from, char byte );
    void append( int from, const char *data, int dataLength );
    void appendFill( int from, char fillDatum, int fillLength );

    void clear();

  public: // get
    QByteArray data( int offset, int length ) const ;
    char operator[]( int offset ) const;

  protected:
    QByteArray mData;
};

inline ChangesDataStorage::ChangesDataStorage() {}

inline QByteArray ChangesDataStorage::data( int offset, int length ) const
{
    return mData.mid( offset, length );
}

inline char ChangesDataStorage::operator[]( int offset ) const { return mData[offset]; }

inline void ChangesDataStorage::clear() { mData.clear(); }

inline void ChangesDataStorage::append( int from, char datum )
{
    mData.resize( from );
    mData.append( datum );
}

inline void ChangesDataStorage::append( int from, const char *data, int dataLength )
{
    mData.resize( from + dataLength );
    memcpy( mData.data()+from, data, dataLength );
}

inline void ChangesDataStorage::appendFill( int from, char fillDatum, int fillLength )
{
    mData.resize( from + fillLength );
    memset( mData.data()+from, fillDatum, fillLength );
}

}


#endif
