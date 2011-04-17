/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_HELPER
#define OKTETA_HELPER

// lib
#include <character.h>
// KDE
#include <KColorScheme>
// Qt
#include <QtGui/QColor>


// temporary solution until syntax highlighting is implemented
static inline QColor colorForChar( const Okteta::Character byteChar )
{
  return byteChar.isUndefined() ? Qt::yellow :
         byteChar.isPunct() ?     Qt::red :
         byteChar.isPrint() ?     Qt::black :
         (byteChar == QLatin1Char(0x0A) || byteChar == QLatin1Char(0x0D)) ?
                                  Qt::darkCyan :
                                  Qt::blue;
}


static inline KColorScheme::ForegroundRole foregroundRoleForChar( const Okteta::Character byteChar )
{
  return byteChar.isUndefined() ? KColorScheme::NegativeText :
         byteChar.isPunct() ?     KColorScheme::InactiveText :
         byteChar.isPrint() ?     KColorScheme::NormalText :
         (byteChar == QLatin1Char(0x0A) || byteChar == QLatin1Char(0x0D)) ?
                                  KColorScheme::VisitedText :
                                  KColorScheme::ActiveText;
}

#endif
