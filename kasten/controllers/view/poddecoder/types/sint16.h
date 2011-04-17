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

#ifndef SINT16_H
#define SINT16_H

// Qt
#include <QtCore/QMetaType>
#include <QtCore/QString>


struct SInt16
{
  public:
    SInt16( qint16 v );
    SInt16();

  public:
    QString toString() const;

  public:
    qint16 value;
};


inline SInt16::SInt16() : value( 0 ) {}
inline SInt16::SInt16( qint16 v ) : value( v ) {}

inline QString SInt16::toString() const { return QString::number( value ); }

Q_DECLARE_METATYPE( SInt16 )

#endif
