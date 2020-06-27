/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "columnsview.hpp"
#include "columnsview_p.hpp"

// Qt
#include <QPaintEvent>
#include <QPainter>
#include <QStyle>
#include <QScrollBar>

namespace Okteta {

ColumnsView::ColumnsView(ColumnsViewPrivate* dd, QWidget* parent)
    : QAbstractScrollArea(parent)
    , d_ptr(dd)
{
    Q_D(ColumnsView);

    d->init();
}

ColumnsView::ColumnsView(/*bool R,*/ QWidget* parent)
    : QAbstractScrollArea(parent)
    , d_ptr(new ColumnsViewPrivate(this))
{
    Q_D(ColumnsView);

    d->init();
}

ColumnsView::~ColumnsView() = default;

LineSize ColumnsView::noOfLines() const
{
    Q_D(const ColumnsView);

    return d->NoOfLines;
}

PixelY ColumnsView::lineHeight() const
{
    Q_D(const ColumnsView);

    return d->LineHeight;
}

Line ColumnsView::lineAt(PixelY y) const
{
    Q_D(const ColumnsView);

    return (d->LineHeight != 0) ? y / d->LineHeight : 0;
}

LineRange ColumnsView::visibleLines() const
{
    const PixelYRange ySpan = PixelYRange::fromWidth(yOffset(), visibleHeight());
    return LineRange(lineAt(ySpan.start()), lineAt(ySpan.end()));
}
LineRange ColumnsView::visibleLines(const PixelYRange& yPixels) const
{
    return LineRange(lineAt(yPixels.start()), lineAt(yPixels.end()));
}

PixelX ColumnsView::visibleWidth() const
{
    return viewport()->width();
}

PixelY ColumnsView::visibleHeight() const
{
    return viewport()->height();
}

PixelY ColumnsView::columnsHeight() const
{
    Q_D(const ColumnsView);

    return d->NoOfLines * d->LineHeight;
}

PixelX ColumnsView::columnsWidth() const
{
    Q_D(const ColumnsView);

    return d->ColumnsWidth;
}

QPoint ColumnsView::viewportToColumns(const QPoint& point) const
{
    return QPoint(xOffset(), yOffset()) + point;
}

PixelX ColumnsView::xOffset() const
{
    return horizontalScrollBar()->value();
}

PixelY ColumnsView::yOffset() const
{
    return verticalScrollBar()->value();
}

PixelY ColumnsView::yOffsetOfLine(Line lineIndex) const
{
    Q_D(const ColumnsView);

    return lineIndex * d->LineHeight - yOffset();
}

void ColumnsView::setColumnsPos(PixelX x, PixelY y)
{
    horizontalScrollBar()->setValue(x);
    verticalScrollBar()->setValue(y);
}

void ColumnsView::setNoOfLines(LineSize newNoOfLines)
{
    Q_D(ColumnsView);

    if (d->NoOfLines == newNoOfLines) {
        return;
    }

    d->NoOfLines = newNoOfLines;

    updateScrollBars();
}

void ColumnsView::setLineHeight(PixelY newLineHeight)
{
    Q_D(ColumnsView);

    if (newLineHeight == d->LineHeight) {
        return;
    }

    d->LineHeight = newLineHeight;

    for (auto column : qAsConst(d->columns)) {
        column->setLineHeight(d->LineHeight);
    }

    verticalScrollBar()->setSingleStep(d->LineHeight);
    updateScrollBars();
}

void ColumnsView::updateWidths()
{
    Q_D(ColumnsView);

    d->updateWidths();

    updateScrollBars();
}

void ColumnsView::updateScrollBars()
{
    QSize viewSize = maximumViewportSize();

    const int scrollBarWidth = style()->pixelMetric(QStyle::PM_ScrollBarExtent);
    const PixelY usedHeight = columnsHeight();
    const PixelX usedWidth = columnsWidth();

    const bool needsVerticalBarDefinitely = (usedHeight > viewSize.height());
    const bool needsHorizontalBarDefinitely = (usedWidth > viewSize.width());

    if (needsVerticalBarDefinitely) {
        viewSize.rwidth() -= scrollBarWidth;
    }
    if (needsHorizontalBarDefinitely) {
        viewSize.rheight() -= scrollBarWidth;
    }
    // check again if bars are not needed now
    if (!needsVerticalBarDefinitely && usedHeight > viewSize.height()) {
        viewSize.rwidth() -= scrollBarWidth;
    }
    if (!needsHorizontalBarDefinitely && usedWidth > viewSize.width()) {
        viewSize.rheight() -= scrollBarWidth;
    }

    verticalScrollBar()->setRange(0, usedHeight - viewSize.height());
    verticalScrollBar()->setPageStep(viewSize.height());
    horizontalScrollBar()->setRange(0, usedWidth - viewSize.width());
    horizontalScrollBar()->setPageStep(viewSize.width());
}

void ColumnsView::updateColumn(AbstractColumnRenderer& columnRenderer)
{
    if (columnRenderer.isVisible()) {
        viewport()->update(columnRenderer.x() - xOffset(), 0, columnRenderer.width(), visibleHeight());
    }
}

void ColumnsView::updateColumn(AbstractColumnRenderer& columnRenderer, const LineRange& lines)
{
    Q_D(ColumnsView);

    if (columnRenderer.isVisible()) { // TODO: catch hidden range && columnRenderer.overlaps(Xs) )
        LineRange linesToUpdate = visibleLines();
        linesToUpdate.restrictTo(lines);
        if (linesToUpdate.isValid()) {
            const PixelX x = columnRenderer.x() - xOffset();
            const PixelY y = yOffsetOfLine(linesToUpdate.start());
            const int width = columnRenderer.width();
            const int height = d->LineHeight * linesToUpdate.width();
            viewport()->update(x, y, width, height);
        }
    }
}

LineSize ColumnsView::noOfLinesPerPage() const
{
    Q_D(const ColumnsView);

    if (d->LineHeight < 1) {
        return 1;
    }

    LineSize result = (visibleHeight() - 1) / d->LineHeight; // -1 ensures to get always the last visible line

    if (result < 1) {
        // ensure to move down at least one line
        result = 1;
    }

    return result;
}

void ColumnsView::addColumn(AbstractColumnRenderer* columnRenderer)
{
    Q_D(ColumnsView);

//   if( Reversed )
//     Columns.prepend( C );
//   else
    d->columns.append(columnRenderer);

    updateWidths();
}

void ColumnsView::removeColumn(AbstractColumnRenderer* columnRenderer)
{
    Q_D(ColumnsView);

    const int columnRendererIndex = d->columns.indexOf(columnRenderer);
    if (columnRendererIndex != -1) {
        d->columns.removeAt(columnRendererIndex);
    }

    delete columnRenderer;

    updateWidths();
}

void ColumnsView::scrollContentsBy(int dx, int dy)
{
    viewport()->scroll(dx, dy);
}

bool ColumnsView::event(QEvent* event)
{
    if (event->type() == QEvent::StyleChange || event->type() == QEvent::LayoutRequest) {
        updateScrollBars();
    }

    return QAbstractScrollArea::event(event);
}

void ColumnsView::resizeEvent(QResizeEvent* event)
{
    updateScrollBars();

    QAbstractScrollArea::resizeEvent(event);
}

void ColumnsView::paintEvent(QPaintEvent* paintEvent)
{
    QAbstractScrollArea::paintEvent(paintEvent);

    const PixelX x = xOffset();
    const PixelY y = yOffset();

    QRect dirtyRect = paintEvent->rect();
    dirtyRect.translate(x, y);

    QPainter painter(viewport());
    painter.translate(-x, -y);

    renderColumns(&painter, dirtyRect.x(), dirtyRect.y(), dirtyRect.width(), dirtyRect.height());
}

void ColumnsView::renderColumns(QPainter* painter, int cx, int cy, int cw, int ch)
{
    Q_D(ColumnsView);

    PixelXRange dirtyXs = PixelXRange::fromWidth(cx, cw);

    // content to be shown?
    if (dirtyXs.startsBefore(d->ColumnsWidth)) {
        PixelYRange dirtyYs = PixelYRange::fromWidth(cy, ch);

        // collect affected columns
        QVector<AbstractColumnRenderer*> dirtyColumns;
        dirtyColumns.reserve(d->columns.size());
        for (auto column : qAsConst(d->columns)) {
            if (column->isVisible() && column->overlaps(dirtyXs)) {
                dirtyColumns.append(column);
            }
        }

        // any lines of any columns to be drawn?
        if (d->NoOfLines > 0) {
            // calculate affected lines
            LineRange dirtyLines = visibleLines(dirtyYs);
            dirtyLines.restrictEndTo(d->NoOfLines - 1);

            if (dirtyLines.isValid()) {
                // paint full columns
                for (auto column : qAsConst(d->columns)) {
                    column->renderColumn(painter, dirtyXs, dirtyYs);
                }

                PixelY cy = dirtyLines.start() * d->LineHeight;
                // qCDebug(LOG_OKTETA_GUI)<<"Dirty lines: "<<dirtyLines.start()<<"-"<<dirtyLines.end();
                // starting painting with the first line
                Line line = dirtyLines.start();
                auto it = dirtyColumns.constBegin();
                AbstractColumnRenderer* column = *it;
                painter->translate(column->x(), cy);

                while (true) {
                    column->renderFirstLine(painter, dirtyXs, line);
                    ++it;
                    if (it == dirtyColumns.constEnd()) {
                        break;
                    }
                    painter->translate(column->width(), 0);
                    column = *it;
                }
                painter->translate(-column->x(), 0);

                // Go through the other lines
                while (true) {
                    ++line;

                    if (line > dirtyLines.end()) {
                        break;
                    }

                    it = dirtyColumns.constBegin();
                    column = *it;
                    painter->translate(column->x(), d->LineHeight);

                    while (true) {
                        column->renderNextLine(painter);
                        ++it;
                        if (it == dirtyColumns.constEnd()) {
                            break;
                        }
                        painter->translate(column->width(), 0);
                        column = *it;
                    }
                    painter->translate(-column->x(), 0);
                }
                cy = dirtyLines.end() * d->LineHeight;

                painter->translate(0, -cy);
            }
        }

        // draw empty columns?
        dirtyYs.setStart(columnsHeight());
        if (dirtyYs.isValid()) {
            for (auto column : qAsConst(dirtyColumns)) {
                column->renderEmptyColumn(painter, dirtyXs, dirtyYs);
            }
        }
    }

    // painter empty rects
    dirtyXs.setStart(d->ColumnsWidth);
    if (dirtyXs.isValid()) {
        renderEmptyArea(painter, dirtyXs.start(), cy, dirtyXs.width(), ch);
    }
}

void ColumnsView::renderEmptyArea(QPainter* painter, int cx, int cy, int cw, int ch)
{
    painter->fillRect(cx, cy, cw, ch, viewport()->palette().brush(QPalette::Base)); // TODO: use stylist here, too
}

}
