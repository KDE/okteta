/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

    virtual ~AbstractColumnRendererPrivate();

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

inline AbstractColumnRendererPrivate::~AbstractColumnRendererPrivate() = default;

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
