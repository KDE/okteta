/***************************************************************************
                          kcolumn.h  -  description
                             -------------------
    begin                : Mit Mai 21 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_KCOLUMN_H
#define KHE_KCOLUMN_H


#include "kadds.h"

class QPainter;

namespace KHE
{

class KColumnsView;

/** base class for columns of the KColumnsView
  *
  *
  *@author Friedrich W. H. Kossebau
  */

class KColumn
{
//    friend class KColumnsView;
  public:
    KColumn( KColumnsView *V );
    virtual ~KColumn() {}


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
    virtual void paintFirstLine( QPainter *P, KPixelX cx, KPixelX cw, int FirstLine );
    /** the actual painting call for a column's line.
      * The default implementation simply paints the background
      */
    virtual void paintNextLine( QPainter *P );


  public: // modification access
    /** sets starting point of the column */
    void setX( KPixelX NewX );
    /** sets visibily */
    void setVisible( bool V );
    /** buffer actual line height in column */
    void setLineHeight( KPixelY H );

  public: // value access
    /** left offset x in pixel */
    KPixelX x() const;
    /** total width in pixel */
    KPixelX width() const;
    /** right offset x in pixel */
    KPixelX rightX() const;
    /** should Column be displayed? */
    bool isVisible() const;
    /** convinience: returns width if visible else 0 */
    KPixelX visibleWidth() const;

  public: // functional logic
    /** true if column overlaps with pixels between x-positions x1, x2 */
    bool overlaps( KPixelX x1, KPixelX x2 ) const;

  protected:
    /** sets width of the column */
    void setWidth( KPixelX W );
    /** */
    void paintBlankLine( QPainter *P ) const;

  protected: // general column data
    /** pointer to the view */
    KColumnsView *View;
    /** should Column be displayed? */
    bool Visible;

    /** buffered value */
    KPixelY LineHeight;

  private:
    /** left offset x in pixel */
    KPixelX m_X;
    /** most right x in pixel */
    KPixelX m_RightX;
    /** total width in pixel */
    KPixelX m_Width;
};


inline KPixelX KColumn::x()            const { return m_X; }
inline KPixelX KColumn::rightX()       const { return m_RightX; }
inline KPixelX KColumn::width()        const { return m_Width; }
inline bool    KColumn::isVisible()    const { return Visible; }
inline KPixelX KColumn::visibleWidth() const { return Visible ? m_Width : 0; }

inline void KColumn::setX( KPixelX NewX )       { m_X = NewX; m_RightX = m_X+m_Width-1; }
inline void KColumn::setWidth( KPixelX W )      { m_Width = W; m_RightX = m_X+m_Width-1; }
inline void KColumn::setVisible( bool V )       { Visible = V; }
inline void KColumn::setLineHeight( KPixelY H ) { LineHeight = H; }

inline bool KColumn::overlaps( KPixelX x1, KPixelX x2 ) const
{ return m_X <= x2 && m_RightX >= x1; }

}

#endif
