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

#ifndef OKTETA_BYTEARRAYCHANGE_H
#define OKTETA_BYTEARRAYCHANGE_H

// lib
#include "oktetacore_export.h"
#include "arraychangemetrics.h"
// Qt
#include <QtCore/QByteArray>

namespace Okteta
{

// TODO: do we need the invalid status?
// TODO: what about grouped changes
// TODO: use change names from original? Only if local are not available
class OKTETACORE_EXPORT ByteArrayChange
{
    friend QDataStream& operator<<( QDataStream& outStream, const ByteArrayChange& change );
    friend QDataStream& operator>>( QDataStream& inStream, ByteArrayChange& change );

  public:
    ByteArrayChange();
    explicit ByteArrayChange( const ArrayChangeMetrics& metrics, const QByteArray& data = QByteArray() );

  public:
    const ArrayChangeMetrics& metrics() const;
    const QByteArray& data() const;

  protected:
    ArrayChangeMetrics mMetrics;
    QByteArray mData;
//     UserId mUserId;
//      mTime;
};


inline ByteArrayChange::ByteArrayChange()
{}

inline ByteArrayChange::ByteArrayChange( const ArrayChangeMetrics& metrics, const QByteArray& data )
  : mMetrics( metrics ),
    mData( data )
{}

inline const ArrayChangeMetrics& ByteArrayChange::metrics() const { return mMetrics; }
inline const QByteArray& ByteArrayChange::data()            const { return mData; }


QDataStream& operator<<( QDataStream& outStream, const ByteArrayChange& change );
QDataStream& operator>>( QDataStream& inStream, ByteArrayChange& change );

inline QDataStream& operator<<( QDataStream& outStream, const ByteArrayChange& change )
{
    outStream << change.mMetrics << change.mData;
    return outStream;
}

inline QDataStream& operator>>( QDataStream& inStream, ByteArrayChange& change )
{
    inStream >> change.mMetrics >> change.mData;
    return inStream;
}

}

#endif
