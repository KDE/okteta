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

#ifndef OKTETA_OFFSETCOLUMNRENDERER_H
#define OKTETA_OFFSETCOLUMNRENDERER_H

// lib
#include "offsetformat.h"
#include "abstractcolumnrenderer.h"
#include "line.h"

class QFontMetrics;


namespace Okteta
{
class ByteArrayTableLayout;

/**
  *@author Friedrich W. H. Kossebau
  */

class OKTETAGUI_EXPORT OffsetColumnRenderer : public AbstractColumnRenderer
{
  public:
    OffsetColumnRenderer( AbstractColumnStylist* stylist, ByteArrayTableLayout* layout, OffsetFormat::Format format );
    virtual ~OffsetColumnRenderer();

  public:  // AbstractColumnRenderer API
    virtual void renderFirstLine( QPainter* painter, const PixelXRange& Xs, Line firstLineIndex );
    virtual void renderNextLine( QPainter* painter );
    virtual void renderColumn( QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys );
    virtual void renderEmptyColumn( QPainter *painter, const PixelXRange &Xs, const PixelYRange &Ys );

  public:
    void setFormat( OffsetFormat::Format format );
    void setFontMetrics( const QFontMetrics& fontMetrics );

  public: // read access
//     int delta() const;
    OffsetFormat::print printFunction() const;


  protected:
    /** recalculates all x values */
    void recalcX();
    /** paints full line */
    void renderLine( QPainter *painter, Line lineIndex );
    /** */
    void renderColumnBackground( QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys );

  protected: // user settings
    ByteArrayTableLayout* mLayout;

  protected: // pixel related
    /** */
    PixelX mOffsetTextWidth;
    /** */
    PixelY mDigitBaseLine;

  protected: // general layout
    OffsetFormat::Format mFormat;

    OffsetFormat::print PrintFunction;

    /** buffer to hold the formatted coding */
    mutable char mCodedOffset[OffsetFormat::MaxFormatWidth+1];

  protected: // firstnext trips related
    /** */
    Line mRenderLineIndex;
};


inline OffsetFormat::print OffsetColumnRenderer::printFunction()    const { return PrintFunction; }

}

#endif
