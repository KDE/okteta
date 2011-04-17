/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#ifndef ABSTRACTCOLUMNFRAMERENDERER_H
#define ABSTRACTCOLUMNFRAMERENDERER_H


// lib
#include "abstractframerenderer.h"
// Okteta gui
#include <kadds.h>
#include <line.h>
// Okteta core
#include <addressrange.h>
// Qt
#include <QtCore/QList>

namespace Okteta {
class AbstractColumnRenderer;
}


class AbstractColumnFrameRendererPrivate;

/** class to render columns with the same lineheight for all lines
  *
  * lines are only completely rendered, so contentHeight <= framesCount*height
  *@author Friedrich W. H. Kossebau
  */

class AbstractColumnFrameRenderer : public AbstractFrameRenderer
{
  public:
    AbstractColumnFrameRenderer();
    virtual ~AbstractColumnFrameRenderer();

  public: // AbstractFrameRenderer API
//     virtual int height() const;
//     virtual int width() const;
    virtual void renderFrame( QPainter* painter, int frameIndex );

  public: // data-wise sizes
    /** returns the number of all lines */
    Okteta::LineSize noOfLines() const;
    /** returns number of fully visible lines, at least 1 (as needed by page down/up)
      * doesn't care about the total height being smaller than the display height
      */
    Okteta::LineSize noOfLinesPerFrame() const;

  public: // pixel-wise sizes
    /** returns the height of each line */
    Okteta::PixelY lineHeight() const;
    /** returns the width of all visible columns together */
    Okteta::PixelX columnsWidth() const;
    /** returns the height of all lines together */
    Okteta::PixelY columnsHeight() const;

  public:
    /**  */
    void setColumnsPos( Okteta::PixelX x, Okteta::PixelY y );

  protected: // our API
    /** draws area without columns in columns coordinates */
    virtual void drawEmptyArea( QPainter* painter, int cx, int cy, int cw, int ch );

  protected: //
    /** sets height of all lines and propagates this information to all columns
      * doesn't update the content size
      * @param NewLineHeight height in pixels
      */
    virtual void setLineHeight( Okteta::PixelY NewLineHeight );
    /** sets the number of lines
      * doesn't update the content size
      * @param NewNoOfLines new number of lines to display
      */
    virtual void setNoOfLines( Okteta::LineSize NewNoOfLines );

  protected:
    /** takes ownership of column renderer */
    void addColumn( Okteta::AbstractColumnRenderer* column );
    void removeColumn( Okteta::AbstractColumnRenderer* column );

  protected: // recalculations
    /** recalculates the positions of the columns and the total width */
    void updateWidths();

  private:
    AbstractColumnFrameRendererPrivate* const d;
};

#endif
