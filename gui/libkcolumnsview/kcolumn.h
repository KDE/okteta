/***************************************************************************
                          kcolumn.h  -  description
                             -------------------
    begin                : Mit Mai 21 2003
    copyright            : (C) 2003,2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_UI_KCOLUMN_H
#define KHE_UI_KCOLUMN_H


// lib
#include "kadds.h"
// commonlib
#include <ksection.h>

class QPainter;


namespace KHEUI
{

class KColumnsView;

class KColumnPrivate;

/** base class for columns of the KColumnsView
  *
  *
  *@author Friedrich W. H. Kossebau
  */

class KColumn
{
//    friend class KColumnsView;
  public:
    explicit KColumn( KColumnsView *V );
    virtual ~KColumn();

  public: // API to be reimplemented in the subclasses
    /** Before an update of the columns view each column that intersects with the area to be painted
      * will be called with this function. As often multiple lines of a column are affected
      * for each lines the same values (like first and last char positions) might be calculated.
      * This function enables a one-time-calculation for such data that must be stored in some
      * class members, though.
      * @param P painter variable
      * @param cx
      * @param cw
      * @param FirstLine no of the first of the range of lines to paint
      */
    virtual void paintFirstLine( QPainter *P, const KPixelXs &Xs, int FirstLine );
    /** the actual painting call for a column's line.
      * The default implementation simply paints the background
      */
    virtual void paintNextLine( QPainter *P );

    /** */
    virtual void paintEmptyColumn( QPainter *P, const KPixelXs &Xs, const KPixelYs &Ys );

  public: // modification access
    /** sets starting point of the column */
    void setX( KPixelX NewX );
    /** sets visibily */
    void setVisible( bool V );
    /** buffer actual line height in column */
    void setLineHeight( KPixelY H );

  public: // value access
    /** */
    KColumnsView *columnsView() const;
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
    void paintBlankLine( QPainter *P ) const;

  private:
     KColumnPrivate * const d;
};

}

#endif
