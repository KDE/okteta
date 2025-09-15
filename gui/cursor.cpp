/*
    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "cursor.hpp"

namespace Okteta {

void Cursor::setSize(PixelX Width, PixelY Height, qreal devicePixelRatio)
{
    const int scaledWidth = Width * devicePixelRatio;
    const int scaledHeight = Height * devicePixelRatio;
    if (scaledWidth != OnPixmap.width() || scaledHeight != OnPixmap.height()) {
        OnPixmap = QPixmap(scaledWidth, scaledHeight);
        OffPixmap = QPixmap(scaledWidth, scaledHeight);
    }
    OnPixmap.setDevicePixelRatio(devicePixelRatio);
    OffPixmap.setDevicePixelRatio(devicePixelRatio);
}

void Cursor::setShape(PixelX X, PixelX W, qreal devicePixelRatio)
{
    CursorX = X;
    ShapeX = X * devicePixelRatio;
    ShapeW = W * devicePixelRatio;
}

}
