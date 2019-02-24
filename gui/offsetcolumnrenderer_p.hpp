/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_OFFSETCOLUMNRENDERER_P_HPP
#define OKTETA_OFFSETCOLUMNRENDERER_P_HPP

// lib
#include "offsetcolumnrenderer.hpp"
#include "abstractcolumnrenderer_p.hpp"

namespace Okteta {

class OffsetColumnRendererPrivate : public AbstractColumnRendererPrivate
{
public:
    OffsetColumnRendererPrivate(OffsetColumnRenderer* q,
                                AbstractColumnStylist* stylist,
                                ByteArrayTableLayout* layout, OffsetFormat::Format format);
    ~OffsetColumnRendererPrivate() override;

public:
    void init();
    
public: // AbstractColumnRenderer API
    void renderFirstLine(QPainter* painter, const PixelXRange& Xs, Line firstLineIndex);
    void renderNextLine(QPainter* painter);
    void renderColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys);
    void renderEmptyColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys);

public:
    void setFormat(OffsetFormat::Format format, const QFontMetrics& fontMetrics);
    void setFontMetrics(const QFontMetrics& fontMetrics);

private:
    /** recalculates all x values */
    void recalcX();
    /** paints full line */
    void renderLine(QPainter* painter, Line lineIndex);
    /** */
    void renderColumnBackground(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys);

public: // user settings
    ByteArrayTableLayout* mLayout;

public: // pixel related
    /** */
    PixelX mOffsetTextWidth = 0;
    /** */
    PixelY mDigitBaseLine = 0;

public: // general layout
    OffsetFormat::Format mFormat;

    OffsetFormat::print PrintFunction;

    /** buffer to hold the formatted coding */
    mutable char mCodedOffset[OffsetFormat::MaxFormatWidth + 1];

public: // firstnext trips related
    /** */
    Line mRenderLineIndex;

private:
    OffsetColumnRenderer* const q_ptr;
    Q_DECLARE_PUBLIC(OffsetColumnRenderer)
};


inline OffsetColumnRendererPrivate::OffsetColumnRendererPrivate(OffsetColumnRenderer* q,
                                                                AbstractColumnStylist* stylist,
                                                                ByteArrayTableLayout* layout, OffsetFormat::Format format)
    : AbstractColumnRendererPrivate(stylist)
    , mLayout(layout)
    , mFormat(format)
    , PrintFunction(OffsetFormat::printFunction(format))
    , q_ptr(q)
{
}

inline OffsetColumnRendererPrivate::~OffsetColumnRendererPrivate() = default;

inline void OffsetColumnRendererPrivate::init()
{
    recalcX();
}

}

#endif
