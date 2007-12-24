/***************************************************************************
                          abstractcolumnrenderer.h  -  description
                             -------------------
    begin                : Mit Mai 21 2003
    copyright            : 2003,2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef ABSTRACTCOLUMNRENDERER_H
#define ABSTRACTCOLUMNRENDERER_H


// Okteta ui
#include <kadds.h>
// Okteta core
#include <ksection.h>

class QPainter;
using namespace KHEUI;


class AbstractColumnFrameRenderer;

class AbstractColumnRendererPrivate;

/** base class for columns of the AbstractColumnRenderersView
  *
  *
  *@author Friedrich W. H. Kossebau
  */

class AbstractColumnRenderer
{
  public:
    explicit AbstractColumnRenderer( AbstractColumnFrameRenderer *columnframeRenderer );
    virtual ~AbstractColumnRenderer();

  public: // API to be reimplemented in the subclasses
    /** Before an update of the columns view each column that intersects with the area to be painted
      * will be called with this function. As often multiple lines of a column are affected
      * for each lines the same values (like first and last char positions) might be calculated.
      * This function enables a one-time-calculation for such data that must be stored in some
      * class members, though.
      * @param painter painter variable
      * @param cx
      * @param cw
      * @param FirstLine no of the first of the range of lines to paint
      */
    virtual void renderFirstLine( QPainter *painter, const KPixelXs &Xs, int firstLine );
    /** the actual painting call for a column's line.
      * The default implementation simply paints the background
      */
    virtual void renderNextLine( QPainter *painter );

    /** */
    virtual void renderColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys );
    /** */
    virtual void renderEmptyColumn( QPainter *painter, const KPixelXs &Xs, const KPixelYs &Ys );

  public: // modification access
    /** sets starting point of the column */
    void setX( KPixelX NewX );
    /** sets visibily */
    void setVisible( bool V );
    /** buffer actual line height in column */
    void setLineHeight( KPixelY H );

  public: // value access
    /** */
    AbstractColumnFrameRenderer *columnFrameRenderer() const;
    /** left offset x in pixel */
    KPixelX x() const;
    /** total width in pixel */
    KPixelX width() const;
    /** right offset x in pixel */
    KPixelX rightX() const;
    /** should Column be displayed? */
    bool isVisible() const;
    /** convenience: returns width if visible else 0 */
    KPixelX visibleWidth() const;
    /** */
    KPixelY lineHeight() const;

  public: // functional logic
    /** true if column overlaps with pixels between x-positions x1, x2 */
    bool overlaps( const KPixelXs &Xs ) const;

  protected:
    /** sets width of the column */
    void setWidth( KPixelX W );
    /** */
    void restrictToXSpan( KPixelXs *Xs ) const;
    /** */
    void renderBlankLine( QPainter *painter ) const;

  private:
     AbstractColumnRendererPrivate * const d;
};

#endif
