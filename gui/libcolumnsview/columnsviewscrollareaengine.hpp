/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2008, 2019, 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_COLUMNSVIEWSCROLLAREAENGINE_HPP
#define OKTETA_COLUMNSVIEWSCROLLAREAENGINE_HPP

// lib
#include "abstractcolumnrenderer.hpp"
#include "linerange.hpp"
// Qt
#include <QAbstractScrollArea>
#include <QEvent>
#include <QScrollBar>
// Std
#include <memory>
#include <utility>
#include <vector>

namespace Okteta {

/**
 * General class for widgets with columns that display different aspects of the same data
 * with the same lineheight for all lines
 *
 * TODO: any better name than Engine?
 */
class ColumnsViewScrollAreaEngine
{
public:
    explicit ColumnsViewScrollAreaEngine(/*bool R,*/ QAbstractScrollArea* parent);
    ColumnsViewScrollAreaEngine(const ColumnsViewScrollAreaEngine&) = delete;
    ColumnsViewScrollAreaEngine(ColumnsViewScrollAreaEngine&&) = delete;

    virtual ~ColumnsViewScrollAreaEngine();

    ColumnsViewScrollAreaEngine& operator=(const ColumnsViewScrollAreaEngine&) = delete;
    ColumnsViewScrollAreaEngine& operator=(ColumnsViewScrollAreaEngine&&) = delete;

public:
    void init();

public: // data-wise sizes
    /** returns the number of all lines */
    [[nodiscard]]
    LineSize noOfLines() const;
    /** returns number of fully visible lines, at least 1 (as needed by page down/up)
     * doesn't care about the total height being smaller than the display height
     */
    [[nodiscard]]
    LineSize noOfLinesPerPage() const;

public: // pixel-wise sizes
    /** returns the height of each line */
    [[nodiscard]]
    PixelY lineHeight() const;
    /** returns the width of all visible columns together */
    [[nodiscard]]
    PixelX columnsWidth() const;
    /** returns the height of all lines together */
    [[nodiscard]]
    PixelY columnsHeight() const;

public: // services
    /** gives the index of the line that would include y in pixel coord.
     * y is not forced to be inside the total height.
     */
    [[nodiscard]]
    Line lineAt(PixelY y) const;
    /** gives the index of the first and the last line that would be visible
     * these lines might not contain anything
     */
    [[nodiscard]]
    LineRange visibleLines() const;
    /** gives the index of the first and the last line that would be visible in the given pixel range
     * these lines might not contain anything
     */
    [[nodiscard]]
    LineRange visibleLines(PixelYRange yPixels) const;

    /** @return visible width of the current view */
    [[nodiscard]]
    PixelX visibleWidth() const;
    /** @return visible height of the current view */
    [[nodiscard]]
    PixelY visibleHeight() const;
    /** @return x offset of the current view */
    [[nodiscard]]
    PixelX xOffset() const;
    /** @return y offset of the current view */
    [[nodiscard]]
    PixelY yOffset() const;

    /** @return y offset of the current view */
    [[nodiscard]]
    PixelY yOffsetOfLine(Line lineIndex) const;

    /** translates the point to coordinates in the columns */
    [[nodiscard]]
    QPoint viewportToColumns(QPoint point) const;

public:
    /**  */
    void setColumnsPos(PixelX x, PixelY y);

public: // QAbstractScrollArea API
    void event(QEvent* event);
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent* paintEvent);
    void scrollContentsBy(int dx, int dy);

public: // our API
    /** draws all columns in columns coordinates */
    virtual void renderColumns(QPainter* painter, int cx, int cy, int cw, int ch);
    /** draws area without columns in columns coordinates */
    virtual void renderEmptyArea(QPainter* painter, int cx, int cy, int cw, int ch);

public:
    /** sets height of all lines and propagates this information to all columns
     * doesn't update the content size
     * @param lineHeight height in pixels
     */
    virtual void setLineHeight(PixelY lineHeight);
    /** sets the number of lines
     * doesn't update the content size
     * @param noOfLines new number of lines to display
     */
    virtual void setNoOfLines(LineSize noOfLines);

public:
    void addColumn(std::unique_ptr<AbstractColumnRenderer>&& columnRenderer);
    void removeColumn(AbstractColumnRenderer* columnRenderer);

public: // recalculations
    /** recalculates the positions of the columns and the total width */
    void updateWidths();
    void updateScrollBars();
    /** calls updateContent for the Column */
    void updateColumn(AbstractColumnRenderer& columnRenderer);
    /** calls updateContent for the Column for the given lines, if needed */
    void updateColumn(AbstractColumnRenderer& columnRenderer, LineRange lines);

public: // calculated
    /** collection of all the columns. All columns will be autodeleted. */
    std::vector<std::unique_ptr<AbstractColumnRenderer>> columns;
    /** the number of lines which the column view has */
    LineSize NoOfLines = 0;
    /** the height of each line in pixels */
    PixelY LineHeight = 0;
    /** the width of all visible columns together */
    PixelX ColumnsWidth = 0;

public:
//    bool Reversed;

protected:
    QAbstractScrollArea* const q_ptr;

private:
    Q_DECLARE_PUBLIC(QAbstractScrollArea)
};


inline ColumnsViewScrollAreaEngine::ColumnsViewScrollAreaEngine(/*bool R,*/ QAbstractScrollArea* parent)
    : q_ptr(parent)
//    Reversed( R )
{
}

inline ColumnsViewScrollAreaEngine::~ColumnsViewScrollAreaEngine() = default;

inline void ColumnsViewScrollAreaEngine::updateWidths()
{
    ColumnsWidth = 0;
    for (const auto& column : std::as_const(columns)) {
        column->setX(ColumnsWidth);
        ColumnsWidth += column->visibleWidth();
    }

    updateScrollBars();
}

inline LineSize ColumnsViewScrollAreaEngine::noOfLines() const
{
    return NoOfLines;
}

inline PixelY ColumnsViewScrollAreaEngine::lineHeight() const
{
    return LineHeight;
}

inline Line ColumnsViewScrollAreaEngine::lineAt(PixelY y) const
{
    return (LineHeight != 0) ? y / LineHeight : 0;
}

inline LineRange ColumnsViewScrollAreaEngine::visibleLines() const
{
    const PixelYRange ySpan = PixelYRange::fromWidth(yOffset(), visibleHeight());
    return LineRange(lineAt(ySpan.start()), lineAt(ySpan.end()));
}

inline LineRange ColumnsViewScrollAreaEngine::visibleLines(PixelYRange yPixels) const
{
    return LineRange(lineAt(yPixels.start()), lineAt(yPixels.end()));
}

inline PixelX ColumnsViewScrollAreaEngine::visibleWidth() const
{
    Q_Q(const QAbstractScrollArea);

    return q->viewport()->width();
}

inline PixelY ColumnsViewScrollAreaEngine::visibleHeight() const
{
    Q_Q(const QAbstractScrollArea);

    return q->viewport()->height();
}

inline PixelY ColumnsViewScrollAreaEngine::columnsHeight() const
{
    return NoOfLines * LineHeight;
}

inline PixelX ColumnsViewScrollAreaEngine::columnsWidth() const
{
    return ColumnsWidth;
}

inline QPoint ColumnsViewScrollAreaEngine::viewportToColumns(QPoint point) const
{
    return QPoint(xOffset(), yOffset()) + point;
}

inline PixelX ColumnsViewScrollAreaEngine::xOffset() const
{
    Q_Q(const QAbstractScrollArea);

    return q->horizontalScrollBar()->value();
}

inline PixelY ColumnsViewScrollAreaEngine::yOffset() const
{
    Q_Q(const QAbstractScrollArea);

    return q->verticalScrollBar()->value();
}

inline PixelY ColumnsViewScrollAreaEngine::yOffsetOfLine(Line lineIndex) const
{
    return lineIndex * LineHeight - yOffset();
}

inline void ColumnsViewScrollAreaEngine::setColumnsPos(PixelX x, PixelY y)
{
    Q_Q(QAbstractScrollArea);

    q->horizontalScrollBar()->setValue(x);
    q->verticalScrollBar()->setValue(y);
}

inline void ColumnsViewScrollAreaEngine::setNoOfLines(LineSize newNoOfLines)
{
    if (NoOfLines == newNoOfLines) {
        return;
    }

    NoOfLines = newNoOfLines;

    updateScrollBars();
}

inline void ColumnsViewScrollAreaEngine::updateColumn(AbstractColumnRenderer& columnRenderer)
{
    Q_Q(QAbstractScrollArea);

    if (columnRenderer.isVisible()) {
        q->viewport()->update(columnRenderer.x() - xOffset(), 0, columnRenderer.width(), visibleHeight());
    }
}

inline void ColumnsViewScrollAreaEngine::updateColumn(AbstractColumnRenderer& columnRenderer, LineRange lines)
{
    Q_Q(QAbstractScrollArea);

    if (columnRenderer.isVisible()) { // TODO: catch hidden range && columnRenderer.overlaps(Xs) )
        LineRange linesToUpdate = visibleLines();
        linesToUpdate.restrictTo(lines);
        if (linesToUpdate.isValid()) {
            const PixelX x = columnRenderer.x() - xOffset();
            const PixelY y = yOffsetOfLine(linesToUpdate.start());
            const int width = columnRenderer.width();
            const int height = LineHeight * linesToUpdate.width();
            q->viewport()->update(x, y, width, height);
        }
    }
}

inline LineSize ColumnsViewScrollAreaEngine::noOfLinesPerPage() const
{
    if (LineHeight < 1) {
        return 1;
    }

    LineSize result = (visibleHeight() - 1) / LineHeight; // -1 ensures to get always the last visible line

    if (result < 1) {
        // ensure to move down at least one line
        result = 1;
    }

    return result;
}

inline void ColumnsViewScrollAreaEngine::addColumn(std::unique_ptr<AbstractColumnRenderer>&& columnRenderer)
{
//   if( Reversed )
//     Columns.prepend( C );
//   else
    columns.emplace_back(std::move(columnRenderer));

    updateWidths();
}

inline void ColumnsViewScrollAreaEngine::removeColumn(AbstractColumnRenderer* columnRenderer)
{
    auto it = std::find_if(columns.begin(), columns.end(), [columnRenderer](const auto& column){
        return (column.get() == columnRenderer);
    });
    if (it == columns.end()) {
        return;
    }

    columns.erase(it);

    updateWidths();
}

inline void ColumnsViewScrollAreaEngine::scrollContentsBy(int dx, int dy)
{
    Q_Q(QAbstractScrollArea);

    q->viewport()->scroll(dx, dy);
}

inline void ColumnsViewScrollAreaEngine::event(QEvent* event)
{
    if (event->type() == QEvent::StyleChange || event->type() == QEvent::LayoutRequest) {
        updateScrollBars();
    }
}

inline void ColumnsViewScrollAreaEngine::resizeEvent(QResizeEvent* resizeEvent)
{
    Q_UNUSED(resizeEvent)

    updateScrollBars();
}

}

#endif
