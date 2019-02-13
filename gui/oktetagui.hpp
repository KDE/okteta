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

#ifndef OKTETAGUI_HPP
#define OKTETAGUI_HPP

// this
#include <okteta/lineposition.hpp>
// ColumnsView
#include <okteta/pixelmetrics.hpp>
// Okteta core
#include <Okteta/Byte>
#include <Okteta/OktetaCore>
// Qt
#include <QChar>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LOG_OKTETA_GUI)

namespace Okteta {

static constexpr LinePosition NoByteFound = -1;

static constexpr unsigned int StartsBefore = 1;
static constexpr unsigned int EndsLater = 2;
static constexpr Byte EmptyByte = ' ';

static constexpr PixelX DefaultCursorWidth = 2;
static constexpr PixelX DefaultByteSpacingWidth = 3;
static constexpr PixelX DefaultGroupSpacingWidth = 9;
static constexpr PixelX DefaultRowSpacingHeight = 0;
static constexpr int DefaultNoOfGroupedBytes = 4;
static constexpr int DefaultNoOfBytesPerLine =  16;

static constexpr int DefaultBinaryGapWidth = 1;

static constexpr int InsertCursorWidth = 2;

static constexpr bool DefaultShowingNonprinting = false;
static constexpr QChar DefaultSubstituteChar =  QLatin1Char('.');
static constexpr QChar DefaultUndefinedChar =   QChar(QChar::ReplacementCharacter);

}

#endif
