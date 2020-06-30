/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "printcolumnstylist.hpp"

// Qt
#include <QWidget>

namespace Okteta {

PrintColumnStylist::PrintColumnStylist() = default;

PrintColumnStylist::~PrintColumnStylist() = default;

const QPalette& PrintColumnStylist::palette() const
{
    return mPalette;
}

}
