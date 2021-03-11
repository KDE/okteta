/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTCOLUMNRENDERER_HPP
#define OKTETA_ABSTRACTCOLUMNRENDERER_HPP

// lib
#include "oktetagui_export.hpp"
#include "pixelmetrics.hpp"
// Qt
#include <QScopedPointer>

class QPainter;

namespace Okteta {

class AbstractColumnStylist;

class AbstractColumnRendererPrivate;

/** base class for columns of the ColumnsView
 *
 *
 * @author Friedrich W. H. Kossebau
*/

class OKTETAGUI_EXPORT AbstractColumnRenderer
{
//    friend class ColumnsView;

protected:
    explicit AbstractColumnRenderer(AbstractColumnRendererPrivate* d);

public:
    explicit AbstractColumnRenderer(AbstractColumnStylist* stylist);
    virtual ~AbstractColumnRenderer();

public: // API to be reimplemented in the subclasses
    /** Before an update of the columns view each column that intersects with the area to be painted
     * will be called with this function. As often multiple lines of a column are affected
     * for each lines the same values (like first and last char positions) might be calculated.
     * This function enables a one-time-calculation for such data that must be stored in some
     * class members, though.
     * @param painter painter variable
     * @param xSpan
     * @param firstLineIndex no of the first of the range of lines to paint
     */
    virtual void renderFirstLine(QPainter* painter, const PixelXRange& xSpan, int firstLineIndex);
    /** the actual painting call for a column's line.
     * The default implementation simply paints the background
     */
    virtual void renderNextLine(QPainter* painter);

    /** */
    virtual void renderColumn(QPainter* painter, const PixelXRange& xSpan, const PixelYRange& ySpan);
    /** */
    virtual void renderEmptyColumn(QPainter* painter, const PixelXRange& xSpan, const PixelYRange& ySpan);

public: // modification access
    /** sets starting point of the column */
    void setX(PixelX x);
    /** sets visibily */
    void setVisible(bool isVisible);
    /** buffer actual line height in column */
    void setLineHeight(PixelY lineHeight);

public: // value access
    /** */
    AbstractColumnStylist* stylist() const;
    /** left offset x in pixel */
    PixelX x() const;
    /** total width in pixel */
    PixelX width() const;
    /** right offset x in pixel */
    PixelX rightX() const;
    /** should Column be displayed? */
    bool isVisible() const;
    /** convenience: returns width if visible else 0 */
    PixelX visibleWidth() const;
    /** */
    PixelY lineHeight() const;

public: // functional logic
    /** true if column overlaps with pixels between x-positions x1, x2 */
    bool overlaps(const PixelXRange& xSpan) const;

protected:
    /** sets width of the column */
    void setWidth(PixelX width);
    /** */
    void restrictToXSpan(PixelXRange* xSpan) const;
    /** */
    void renderBlankLine(QPainter* painter) const;

protected:
    const QScopedPointer<AbstractColumnRendererPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractColumnRenderer)
};

}

#endif
