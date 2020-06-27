/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_PIXELMETRICS_HPP
#define OKTETA_PIXELMETRICS_HPP

// Okteta core
#include <Okteta/NumberRange>

namespace Okteta {

// these are to emphasize that measuring unit is pixel and which direction
using PixelX = int;
using PixelY = int;

using PixelXRange = NumberRange<PixelX>;
using PixelYRange = NumberRange<PixelY>;

}

#endif
