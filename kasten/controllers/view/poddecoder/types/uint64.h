/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef UINT64_H
#define UINT64_H

// Qt
#include <QtCore/QMetaType>
#include <QtCore/QString>


struct UInt64
{
  public:
    UInt64( quint64 v );
    UInt64();

  public:
    QString toString( bool asHex ) const;

  public:
    quint64 value;
};


inline UInt64::UInt64() : value( 0 ) {}
inline UInt64::UInt64( quint64 v ) : value( v ) {}

inline QString UInt64::toString( bool asHex ) const
{
    return asHex ? QString::fromLatin1( "0x%1" ).arg( value, 16, 16, QChar::fromLatin1('0') ) :
                   QString::number( value );
}


Q_DECLARE_METATYPE( UInt64 )

#endif
