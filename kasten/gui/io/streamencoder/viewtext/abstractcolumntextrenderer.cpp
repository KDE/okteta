/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractcolumntextrenderer.hpp"

// Qt
#include <QString>

namespace Kasten {

AbstractColumnTextRenderer::~AbstractColumnTextRenderer() = default;

QString AbstractColumnTextRenderer::whiteSpace(unsigned int length)
{
    return {static_cast<int>(length), QLatin1Char(' ')};
}

int AbstractColumnTextRenderer::noOfSublinesNeeded() const { return DefaultNoOfSublines; }

}
