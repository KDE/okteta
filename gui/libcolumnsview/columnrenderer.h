/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2007,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KHE_UI_COLUMNRENDERER_H
#define KHE_UI_COLUMNRENDERER_H


// lib
#include "kadds.h"
// commonlib
#include <ksection.h>

class QPainter;


namespace KHEUI
{

class ColumnsView;

class ColumnRendererPrivate;

/** base class for columns of the ColumnsView
  *
  *
  *@author Friedrich W. H. Kossebau
  */

class ColumnRenderer
{
//    friend class ColumnsView;
  public:
    explicit ColumnRenderer( ColumnsView *columnsView );
    virtual ~ColumnRenderer();

  public: // API to be reimplemented in the subclasses
    /** Before an update of the columns view each column that intersects with the area to be painted
      * will be called with this function. As often multiple lines of a column are affected
      * for each lines the same values (like first and last char positions) might be calculated.
      * This function enables a one-time-calculation for such data that must be stored in some
      * class members, though.
      * @param painter painter variable
      * @param cx
      * @param cw
      * @param firstLineIndex no of the first of the range of lines to paint
      */
    virtual void renderFirstLine( QPainter *painter, const KPixelXs &Xs, int firstLineIndex );
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
    void setX( KPixelX newX );
    /** sets visibily */
    void setVisible( bool visible );
    /** buffer actual line height in column */
    void setLineHeight( KPixelY lineHeight );

  public: // value access
    /** */
    ColumnsView *columnsView() const;
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
    void setWidth( KPixelX width );
    /** */
    void restrictToXSpan( KPixelXs *Xs ) const;
    /** */
    void renderBlankLine( QPainter *painter ) const;

  private:
     ColumnRendererPrivate * const d;
};

}

#endif
