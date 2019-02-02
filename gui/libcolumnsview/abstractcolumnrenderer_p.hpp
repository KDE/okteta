/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_ABSTRACTCOLUMNRENDERER_P_HPP
#define OKTETA_ABSTRACTCOLUMNRENDERER_P_HPP

#include "abstractcolumnrenderer.hpp"

// lib
#include "abstractcolumnstylist.hpp"
// Qt
#include <QPainter>
#include <QPalette>

namespace Okteta {

class AbstractColumnRendererPrivate
{
public:
    explicit AbstractColumnRendererPrivate(AbstractColumnStylist* stylist);
    AbstractColumnRendererPrivate() = delete;

public:
    void renderBlankLine(QPainter* painter) const;
    void renderEmptyColumn(QPainter* painter, const PixelXRange& xSpan, const PixelYRange& ySpan);

public: // general column data
    /** pointer to the view */
    AbstractColumnStylist* mStylist;
    /** should Column be displayed? */
    bool mIsVisible = true;  // TODO: would false be better?

    /** buffered value */
    PixelY mLineHeight = 0;

    /** span of the column in pixel */
    PixelXRange mXSpan = PixelXRange::fromWidth(0, 0);
};

inline AbstractColumnRendererPrivate::AbstractColumnRendererPrivate(AbstractColumnStylist* stylist)
    : mStylist(stylist)
{
}

inline void AbstractColumnRendererPrivate::renderBlankLine(QPainter* painter) const
{
    if (mLineHeight > 0) {
        painter->fillRect(0, 0, mXSpan.width(), mLineHeight,
                          mStylist->palette().brush(QPalette::Base));
    }
}

inline void AbstractColumnRendererPrivate::renderEmptyColumn(QPainter* painter, const PixelXRange& _xSpan, const PixelYRange& ySpan)
{
    PixelXRange xSpan(_xSpan);
    xSpan.restrictTo(mXSpan);

    painter->fillRect(xSpan.start(), ySpan.start(), xSpan.width(), ySpan.width(),
                      mStylist->palette().brush(QPalette::Base));
}

}

#endif
