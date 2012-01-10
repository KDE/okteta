/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2010-2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETAGUI_H
#define OKTETAGUI_H

// this
#include "lineposition.h"
// ColumnsView
#include "kadds.h"
// Okteta core
#include "byte.h"
#include "oktetacore.h"
// Qt
#include <QtCore/QChar>

namespace Okteta
{

static const LinePosition NoByteFound = -1;

static const unsigned int StartsBefore = 1;
static const unsigned int EndsLater = 2;
static const Byte EmptyByte = ' ';

static const PixelX DefaultCursorWidth = 2;
static const PixelX DefaultByteSpacingWidth = 3;
static const PixelX DefaultGroupSpacingWidth = 9;
static const PixelX DefaultRowSpacingHeight = 0;
static const int DefaultNoOfGroupedBytes = 4;
static const int DefaultNoOfBytesPerLine =  16;

static const int DefaultBinaryGapWidth = 1;

static const int InsertCursorWidth = 2;

static const bool  DefaultShowingNonprinting = false;
static const QChar DefaultSubstituteChar =  QLatin1Char( '.' );
static const QChar DefaultUndefinedChar =   QChar( QChar::ReplacementCharacter );

}

#endif
