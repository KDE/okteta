/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_COLUMNSVIEW_HPP
#define OKTETA_COLUMNSVIEW_HPP

// lib
#include "oktetagui_export.hpp"
#include "pixelmetrics.hpp"
#include "linerange.hpp"
// Qt
#include <QAbstractScrollArea>

namespace Okteta {

class AbstractColumnRenderer;
class ColumnsViewPrivate;

/** general class for widgets with columns that display different aspects of the same data
 * with the same lineheight for all lines
 *
 * @author Friedrich W. H. Kossebau
 */

class OKTETAGUI_EXPORT ColumnsView : public QAbstractScrollArea
{
    Q_OBJECT

protected:
    explicit ColumnsView(ColumnsViewPrivate* d, QWidget* parent = nullptr);

public:
    explicit ColumnsView(/*bool R = false,*/ QWidget* parent = nullptr);
    ~ColumnsView() override;

public: // data-wise sizes
    /** returns the number of all lines */
    LineSize noOfLines() const;
    /** returns number of fully visible lines, at least 1 (as needed by page down/up)
     * doesn't care about the total height being smaller than the display height
     */
    LineSize noOfLinesPerPage() const;

public: // pixel-wise sizes
    /** returns the height of each line */
    PixelY lineHeight() const;
    /** returns the width of all visible columns together */
    PixelX columnsWidth() const;
    /** returns the height of all lines together */
    PixelY columnsHeight() const;

public: // services
    /** gives the index of the line that would include y in pixel coord.
     * y is not forced to be inside the total height.
     */
    Line lineAt(PixelY y) const;
    /** gives the index of the first and the last line that would be visible
     * these lines might not contain anything
     */
    LineRange visibleLines() const;
    /** gives the index of the first and the last line that would be visible in the given pixel range
     * these lines might not contain anything
     */
    LineRange visibleLines(const PixelYRange& yPixels) const;

    /** @return visible width of the current view */
    PixelX visibleWidth() const;
    /** @return visible height of the current view */
    PixelY visibleHeight() const;
    /** @return x offset of the current view */
    PixelX xOffset() const;
    /** @return y offset of the current view */
    PixelY yOffset() const;

    /** @return y offset of the current view */
    PixelY yOffsetOfLine(Line lineIndex) const;

    /** translates the point to coordinates in the columns */
    QPoint viewportToColumns(const QPoint& point) const;

public:
    /**  */
    void setColumnsPos(PixelX x, PixelY y);

protected: // QAbstractScrollArea API
    bool event(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* paintEvent) override;
    void scrollContentsBy(int dx, int dy) override;

protected: // our API
    /** draws all columns in columns coordinates */
    virtual void renderColumns(QPainter* painter, int cx, int cy, int cw, int ch);
    /** draws area without columns in columns coordinates */
    virtual void renderEmptyArea(QPainter* painter, int cx, int cy, int cw, int ch);

protected:
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

protected:
    void addColumn(AbstractColumnRenderer* columnRenderer);
    void removeColumn(AbstractColumnRenderer* columnRenderer);

protected: // recalculations
    /** recalculates the positions of the columns and the total width */
    void updateWidths();
    void updateScrollBars();
    /** calls updateContent for the Column */
    void updateColumn(AbstractColumnRenderer& columnRenderer);
    /** calls updateContent for the Column for the given lines, if needed */
    void updateColumn(AbstractColumnRenderer& columnRenderer, const LineRange& lines);

protected:
    const QScopedPointer<ColumnsViewPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(ColumnsView)
};

}

#endif
