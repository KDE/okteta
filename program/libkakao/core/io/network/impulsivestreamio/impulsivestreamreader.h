/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#ifndef IMPULSIVESTREAMREADER_H
#define IMPULSIVESTREAMREADER_H

// Kakao core
#include <abstractimpulsivestreamreader.h>
// Qt
#include <QtCore/QIODevice>


template<typename T>
class ImpulsiveStreamReader : public AbstractImpulsiveStreamReader
{
  public:
    static const int DataSize = sizeof(T);

  public:
    ImpulsiveStreamReader();
    virtual ~ImpulsiveStreamReader();

  public: // AbstractImpulsiveStreamReader API
    virtual int nextBytesNeeded() const;
    virtual bool readFrom( QIODevice* ioDevice );
    virtual void reset();

  public:
    T data() const;

  protected:
    T mData;
    bool mHasRead;
};


template<typename T>
inline ImpulsiveStreamReader<T>::ImpulsiveStreamReader()
: AbstractImpulsiveStreamReader(),
  mHasRead( false )
{}

template<typename T>
inline T ImpulsiveStreamReader<T>::data() const { return mData; }

template<typename T>
inline int ImpulsiveStreamReader<T>::nextBytesNeeded() const
{
    return ( mHasRead ? 0 : DataSize );
}

template<typename T>
inline bool ImpulsiveStreamReader<T>::readFrom( QIODevice* ioDevice )
{
    const qint64 readCount = ioDevice->read( (char*)&mData, DataSize );

    const bool success = ( readCount != -1 );
    if( success )
        mHasRead = true;

    return success;
}

template<typename T>
inline void ImpulsiveStreamReader<T>::reset()
{
    mHasRead = false;
}

template<typename T>
inline ImpulsiveStreamReader<T>::~ImpulsiveStreamReader() {}

#endif
