/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractcolumnframerenderer.hpp"

// Okteta gui
#include <Okteta/LineRange>
#include <Okteta/AbstractColumnRenderer>
// Qt
#include <QPainter>
// Std
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

class AbstractColumnFrameRendererPrivate
{
public:
    AbstractColumnFrameRendererPrivate();
    ~AbstractColumnFrameRendererPrivate();

public:
    void updateWidths();

public: // calculated
    /** collection of all the columns. All columns will be autodeleted. */
    std::vector<std::unique_ptr<Okteta::AbstractColumnRenderer>> mColumns;
    /** the number of lines which the columnRenderer view has */
    Okteta::LineSize mNoOfLines = 0;
    /** the height of each line in pixels */
    Okteta::PixelY mLineHeight = 1;
    /** the width of all visible columns together */
    Okteta::PixelX mColumnsWidth = 0;
};

AbstractColumnFrameRendererPrivate::AbstractColumnFrameRendererPrivate() = default;

AbstractColumnFrameRendererPrivate::~AbstractColumnFrameRendererPrivate() = default;

void AbstractColumnFrameRendererPrivate::updateWidths()
{
    mColumnsWidth = 0;
    for (auto& columnRenderer : std::as_const(mColumns)) {
        columnRenderer->setX(mColumnsWidth);
        mColumnsWidth += columnRenderer->visibleWidth();
    }
}

AbstractColumnFrameRenderer::AbstractColumnFrameRenderer()
    : d(new AbstractColumnFrameRendererPrivate())
{
}

AbstractColumnFrameRenderer::~AbstractColumnFrameRenderer() = default;

Okteta::LineSize AbstractColumnFrameRenderer::noOfLines()          const { return d->mNoOfLines; }
Okteta::PixelY AbstractColumnFrameRenderer::lineHeight()     const { return d->mLineHeight; }

Okteta::PixelY AbstractColumnFrameRenderer::columnsHeight() const { return d->mNoOfLines * d->mLineHeight; }
Okteta::PixelX AbstractColumnFrameRenderer::columnsWidth()  const { return d->mColumnsWidth; }

void AbstractColumnFrameRenderer::setNoOfLines(Okteta::LineSize noOfLines)
{
    if (d->mNoOfLines == noOfLines) {
        return;
    }

    d->mNoOfLines = noOfLines;
}

void AbstractColumnFrameRenderer::setLineHeight(Okteta::PixelY newLineHeight)
{
    if (newLineHeight == d->mLineHeight) {
        return;
    }

    if (newLineHeight < 1) {
        newLineHeight = 1;
    }
    d->mLineHeight = newLineHeight;

    for (auto& columnRenderer : std::as_const(d->mColumns)) {
        columnRenderer->setLineHeight(d->mLineHeight);
    }
}

void AbstractColumnFrameRenderer::updateWidths()
{
    d->updateWidths();
}

Okteta::LineSize AbstractColumnFrameRenderer::noOfLinesPerFrame() const
{
    // TODO: the right reaction?
    if (d->mLineHeight < 1) {
        return 1;
    }

    Okteta::LineSize noOfLinesPerFrame = height() / d->mLineHeight;

    if (noOfLinesPerFrame < 1) {
        // ensure at least one line, so there aren't endless frames TODO: think about
        noOfLinesPerFrame = 1;
    }

    return noOfLinesPerFrame;
}

void AbstractColumnFrameRenderer::addColumn(Okteta::AbstractColumnRenderer* columnRenderer)
{
    d->mColumns.emplace_back(columnRenderer);

    updateWidths();
}

void AbstractColumnFrameRenderer::removeColumn(Okteta::AbstractColumnRenderer* columnRenderer)
{
    auto it = std::find_if(d->mColumns.begin(), d->mColumns.end(), [columnRenderer](const auto& column) {
        return column.get() == columnRenderer;
    });
    if (it == d->mColumns.end()) {
        return;
    }
    d->mColumns.erase(it);

    updateWidths();
}

void AbstractColumnFrameRenderer::renderFrame(QPainter* painter, int frameIndex)
{
    Okteta::PixelXRange renderedXs = Okteta::PixelXRange::fromWidth(0, width());

    // content to be shown?
    if (renderedXs.startsBefore(d->mColumnsWidth)) {
        // collect affected columns
        std::vector<Okteta::AbstractColumnRenderer*> columnRenderers;
        columnRenderers.reserve(d->mColumns.size());
        for (auto& columnRenderer : std::as_const(d->mColumns)) {
            if (columnRenderer->isVisible() && columnRenderer->overlaps(renderedXs)) {
                columnRenderers.emplace_back(columnRenderer.get());
            }
        }

        // calculate affected lines
        const Okteta::Line baseLine = frameIndex * noOfLinesPerFrame();
        Okteta::LineRange renderedLines = Okteta::LineRange::fromWidth(baseLine, noOfLinesPerFrame());
        renderedLines.restrictEndTo(noOfLines() - 1);

        Okteta::PixelYRange renderedYs = Okteta::PixelYRange::fromWidth(0, renderedLines.width() * d->mLineHeight);

        // any lines of any columns to be drawn?
        if (renderedLines.isValid()) {
            // paint full columns
            for (auto* const columnRenderer : columnRenderers) {
                columnRenderer->renderColumn(painter, renderedXs, renderedYs);
            }

            Okteta::PixelY cy = 0;
            // starting painting with the first line
            Okteta::Line line = renderedLines.start();
            auto it = columnRenderers.cbegin();
            Okteta::AbstractColumnRenderer* columnRenderer = *it;
            painter->translate(columnRenderer->x(), cy);

            while (true) {
                columnRenderer->renderFirstLine(painter, renderedXs, line);
                ++it;
                if (it == columnRenderers.cend()) {
                    break;
                }
                painter->translate(columnRenderer->width(), 0);
                columnRenderer = *it;
            }
            painter->translate(-columnRenderer->x(), 0);

            // Go through the other lines
            while (true) {
                ++line;

                if (line > renderedLines.end()) {
                    break;
                }

                auto it = columnRenderers.cbegin();
                columnRenderer = *it;
                painter->translate(columnRenderer->x(), d->mLineHeight);

                while (true) {
                    columnRenderer->renderNextLine(painter);
                    ++it;
                    if (it == columnRenderers.cend()) {
                        break;
                    }
                    painter->translate(columnRenderer->width(), 0);
                    columnRenderer = *it;
                }
                painter->translate(-columnRenderer->x(), 0);
            }
            cy = (renderedLines.width() - 1) * d->mLineHeight;

            painter->translate(0, -cy);
        }

        // draw empty columns?
        renderedYs.set(renderedYs.nextBehindEnd(), height() - 1);
        if (renderedYs.isValid()) {
            for (auto* const columnRenderer : columnRenderers) {
                columnRenderer->renderEmptyColumn(painter, renderedXs, renderedYs);
            }
        }
    }

    // painter empty rects
    renderedXs.setStart(d->mColumnsWidth);
    if (renderedXs.isValid()) {
        drawEmptyArea(painter, renderedXs.start(), 0, renderedXs.width(), height());
    }
}

void AbstractColumnFrameRenderer::drawEmptyArea(QPainter* painter, int cx, int cy, int cw, int ch)
{
    painter->fillRect(cx, cy, cw, ch, Qt::white);
}
