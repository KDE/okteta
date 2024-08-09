/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2019, 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "columnsviewscrollareaengine.hpp"

// lib
#include <logging.hpp>
// Qt
#include <QPainter>
#include <QPaintEvent>
// Std
#include <vector>

namespace Okteta {

static constexpr int DefaultSingleStep = 20;

void ColumnsViewScrollAreaEngine::init()
{
    Q_Q(QAbstractScrollArea);

    QWidget* viewport = q->viewport();
    viewport->setAttribute(Qt::WA_StaticContents);
    viewport->setBackgroundRole(QPalette::Base);
    q->horizontalScrollBar()->setSingleStep(DefaultSingleStep);
    q->verticalScrollBar()->setSingleStep(DefaultSingleStep);

    viewport->setFocusProxy(q);
    viewport->setFocusPolicy(Qt::WheelFocus);
}

void ColumnsViewScrollAreaEngine::renderColumns(QPainter* painter, int cx, int cy, int cw, int ch)
{
    PixelXRange dirtyXs = PixelXRange::fromWidth(cx, cw);

    // content to be shown?
    if (dirtyXs.startsBefore(ColumnsWidth)) {
        PixelYRange dirtyYs = PixelYRange::fromWidth(cy, ch);

        // collect affected columns
        std::vector<AbstractColumnRenderer*> dirtyColumns;
        dirtyColumns.reserve(columns.size());
        for (const auto& column : columns) {
            if (column->isVisible() && column->overlaps(dirtyXs)) {
                dirtyColumns.emplace_back(column.get());
            }
        }

        // any lines of any columns to be drawn?
        if (NoOfLines > 0) {
            // calculate affected lines
            LineRange dirtyLines = visibleLines(dirtyYs);
            dirtyLines.restrictEndTo(NoOfLines - 1);

            if (dirtyLines.isValid()) {
                // paint full columns
                for (const auto& column : columns) {
                    column->renderColumn(painter, dirtyXs, dirtyYs);
                }

                PixelY cy = dirtyLines.start() * LineHeight;
                // qCDebug(LOG_OKTETA_GUI)<<"Dirty lines: "<<dirtyLines.start()<<"-"<<dirtyLines.end();
                // starting painting with the first line
                Line line = dirtyLines.start();
                auto it = dirtyColumns.cbegin();
                AbstractColumnRenderer* column = *it;
                painter->translate(column->x(), cy);

                while (true) {
                    column->renderFirstLine(painter, dirtyXs, line);
                    ++it;
                    if (it == dirtyColumns.cend()) {
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

                    it = dirtyColumns.cbegin();
                    column = *it;
                    painter->translate(column->x(), LineHeight);

                    while (true) {
                        column->renderNextLine(painter);
                        ++it;
                        if (it == dirtyColumns.cend()) {
                            break;
                        }
                        painter->translate(column->width(), 0);
                        column = *it;
                    }
                    painter->translate(-column->x(), 0);
                }
                cy = dirtyLines.end() * LineHeight;

                painter->translate(0, -cy);
            }
        }

        // draw empty columns?
        dirtyYs.setStart(columnsHeight());
        if (dirtyYs.isValid()) {
            for (auto* const column : dirtyColumns) {
                column->renderEmptyColumn(painter, dirtyXs, dirtyYs);
            }
        }
    }

    // painter empty rects
    dirtyXs.setStart(ColumnsWidth);
    if (dirtyXs.isValid()) {
        renderEmptyArea(painter, dirtyXs.start(), cy, dirtyXs.width(), ch);
    }
}

void ColumnsViewScrollAreaEngine::renderEmptyArea(QPainter* painter, int cx, int cy, int cw, int ch)
{
    Q_Q(QAbstractScrollArea);

    painter->fillRect(cx, cy, cw, ch, q->viewport()->palette().brush(QPalette::Base)); // TODO: use stylist here, too
}

void ColumnsViewScrollAreaEngine::setLineHeight(PixelY newLineHeight)
{
    Q_Q(QAbstractScrollArea);

    if (newLineHeight == LineHeight) {
        return;
    }

    LineHeight = newLineHeight;

    for (const auto& column : columns) {
        column->setLineHeight(LineHeight);
    }

    q->verticalScrollBar()->setSingleStep(LineHeight);

    updateScrollBars();
}

void ColumnsViewScrollAreaEngine::updateScrollBars()
{
    Q_Q(QAbstractScrollArea);

    QSize viewSize = q->maximumViewportSize();

    const int scrollBarWidth = q->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
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

    q->verticalScrollBar()->setRange(0, usedHeight - viewSize.height());
    q->verticalScrollBar()->setPageStep(viewSize.height());
    q->horizontalScrollBar()->setRange(0, usedWidth - viewSize.width());
    q->horizontalScrollBar()->setPageStep(viewSize.width());
}

void ColumnsViewScrollAreaEngine::paintEvent(QPaintEvent* paintEvent)
{
    Q_Q(QAbstractScrollArea);

    const PixelX x = xOffset();
    const PixelY y = yOffset();

    QRect dirtyRect = paintEvent->rect();
    dirtyRect.translate(x, y);

    QPainter painter(q->viewport());
    painter.translate(-x, -y);

    renderColumns(&painter, dirtyRect.x(), dirtyRect.y(), dirtyRect.width(), dirtyRect.height());
}

}
