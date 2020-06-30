/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTCOLUMNFRAMERENDERER_HPP
#define KASTEN_ABSTRACTCOLUMNFRAMERENDERER_HPP

// lib
#include "abstractframerenderer.hpp"
// Okteta gui
#include <Okteta/PixelMetrics>
#include <Okteta/Line>
// Okteta core
#include <Okteta/AddressRange>

namespace Okteta {
class AbstractColumnRenderer;
}

/** class to render columns with the same lineheight for all lines
 *
 * lines are only completely rendered, so contentHeight <= framesCount*height
 * @author Friedrich W. H. Kossebau
 */

class AbstractColumnFrameRenderer : public AbstractFrameRenderer
{
public:
    AbstractColumnFrameRenderer();
    ~AbstractColumnFrameRenderer() override;

public: // AbstractFrameRenderer API
//     virtual int height() const;
//     virtual int width() const;
    void renderFrame(QPainter* painter, int frameIndex) override;

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
    void setColumnsPos(Okteta::PixelX x, Okteta::PixelY y);

protected: // our API
    /** draws area without columns in columns coordinates */
    virtual void drawEmptyArea(QPainter* painter, int cx, int cy, int cw, int ch);

protected:
    /** sets height of all lines and propagates this information to all columns
     * doesn't update the content size
     * @param NewLineHeight height in pixels
     */
    virtual void setLineHeight(Okteta::PixelY NewLineHeight);
    /** sets the number of lines
     * doesn't update the content size
     * @param NewNoOfLines new number of lines to display
     */
    virtual void setNoOfLines(Okteta::LineSize NewNoOfLines);

protected:
    /** takes ownership of column renderer */
    void addColumn(Okteta::AbstractColumnRenderer* column);
    void removeColumn(Okteta::AbstractColumnRenderer* column);

protected: // recalculations
    /** recalculates the positions of the columns and the total width */
    void updateWidths();

private:
    QScopedPointer<class AbstractColumnFrameRendererPrivate> const d;
};

#endif
