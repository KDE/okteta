/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CURSOR_HPP
#define OKTETA_CURSOR_HPP

// lib
#include "pixelmetrics.hpp"
// Qt
#include <QPixmap>

namespace Okteta {

/**
 * @author Friedrich W. H. Kossebau
 */
class Cursor
{
public:
    Cursor();
    virtual ~Cursor();

public:
    /** sets size of the full cursor */
    void setSize(PixelX Width, PixelY Height, qreal devicePixelRatio);
    /** sets the shape of the cursor to be drawn */
    void setShape(PixelX X, PixelX W, qreal devicePixelRatio);

public: // access
    QPixmap& onPixmap();
    QPixmap& offPixmap();
    PixelX cursorX() const;
    PixelX shapeX() const;
    PixelX shapeW() const;

private:
    QPixmap OnPixmap;
    QPixmap OffPixmap;

    PixelX CursorX = 0;
    PixelX ShapeX = -1;
    PixelX ShapeW = -1;
};

inline QPixmap& Cursor::onPixmap()  { return OnPixmap; }
inline QPixmap& Cursor::offPixmap() { return OffPixmap; }

inline PixelX Cursor::cursorX() const { return CursorX; }
inline PixelX Cursor::shapeX() const { return ShapeX; }
inline PixelX Cursor::shapeW() const { return ShapeW; }

}

#endif
