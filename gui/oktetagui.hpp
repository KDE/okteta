/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2010-2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETAGUI_HPP
#define OKTETAGUI_HPP

// this
#include "lineposition.hpp"
// ColumnsView
#include "pixelmetrics.hpp"
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
