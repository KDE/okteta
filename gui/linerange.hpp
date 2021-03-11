/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_LINERANGE_HPP
#define OKTETA_LINERANGE_HPP

// lib
#include "line.hpp"
// Okteta core
#include <Okteta/NumberRange>

namespace Okteta {

using LineRange = NumberRange<Line, LineSize>;

}

#endif
