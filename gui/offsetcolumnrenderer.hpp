/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_OFFSETCOLUMNRENDERER_HPP
#define OKTETA_OFFSETCOLUMNRENDERER_HPP

// lib
#include <okteta/offsetformat.hpp>
#include <okteta/abstractcolumnrenderer.hpp>
#include <okteta/line.hpp>

class QFontMetrics;

namespace Okteta {
class ByteArrayTableLayout;

/**
 * @author Friedrich W. H. Kossebau
 */

class OKTETAGUI_EXPORT OffsetColumnRenderer : public AbstractColumnRenderer
{
public:
    OffsetColumnRenderer(AbstractColumnStylist* stylist, ByteArrayTableLayout* layout, OffsetFormat::Format format);
    OffsetColumnRenderer() = delete;

    ~OffsetColumnRenderer() override;

public:    // AbstractColumnRenderer API
    void renderFirstLine(QPainter* painter, const PixelXRange& Xs, Line firstLineIndex) override;
    void renderNextLine(QPainter* painter) override;
    void renderColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys) override;
    void renderEmptyColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys) override;

public:
    void setFormat(OffsetFormat::Format format, const QFontMetrics& fontMetrics);
    OKTETAGUI_DEPRECATED void setFormat(OffsetFormat::Format format);
    void setFontMetrics(const QFontMetrics& fontMetrics);

public: // read access
//     int delta() const;
    OffsetFormat::Format format() const;
    OffsetFormat::print printFunction() const;

protected:
    /** recalculates all x values */
    void recalcX();
    /** paints full line */
    void renderLine(QPainter* painter, Line lineIndex);
    /** */
    void renderColumnBackground(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys);

private: // user settings
    ByteArrayTableLayout* mLayout;

private: // pixel related
    /** */
    PixelX mOffsetTextWidth = 0;
    /** */
    PixelY mDigitBaseLine = 0;

private: // general layout
    OffsetFormat::Format mFormat;

    OffsetFormat::print PrintFunction;

    /** buffer to hold the formatted coding */
    mutable char mCodedOffset[OffsetFormat::MaxFormatWidth + 1];

private: // firstnext trips related
    /** */
    Line mRenderLineIndex;
};

inline OffsetFormat::print OffsetColumnRenderer::printFunction()    const { return PrintFunction; }
inline OffsetFormat::Format OffsetColumnRenderer::format()          const { return mFormat; }

}

#endif
