/***************************************************************************
                          kcolumnsview.h  -  description
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


#ifndef KHE_KCOLUMNSVIEW_H
#define KHE_KCOLUMNSVIEW_H

// qt specific
#include <qptrlist.h>
#include <qwidget.h>
#include <qpixmap.h>
#include <qscrollview.h>
// lib specific
#include "kadds.h"
#include "ksection.h" // TODO: think about moving this out of the public API

namespace KHE
{

class KColumn;
class KColumnsViewPrivate;

/** general class for widgets with columns that display different aspects of the same data
  * with the same lineheight for all lines
  *
  *@author Friedrich W. H. Kossebau
  */

class KColumnsView : public QScrollView
{
   Q_OBJECT

   friend class KColumn;

  public:
    KColumnsView( /*bool R = false,*/ QWidget *parent=0, const char *name=0, WFlags Flags=0 );
    virtual ~KColumnsView();

  public: // drawing
    virtual void paintEmptyArea( QPainter *p, int cx, int cy, int cw, int ch );
    virtual void drawContents( QPainter *p, int cx, int cy, int cw, int ch );

  public: // data-wise sizes
    /** returns the number of all lines */
    int noOfLines() const;
    /** returns number of fully visible lines, at least 1 (as needed by page down/up)
      * doesn't care about the total height being smaller than the display height
      */
    int noOfLinesPerPage() const;

  public: // pixel-wise sizes
    /** returns the height of each line */
    KPixelY lineHeight() const;
    /** returns the size of all visible columns together */
    QSize totalViewSize() const;
    /** returns the width of all visible columns together */
    KPixelX totalWidth() const;
    /** returns the height of all lines together */
    KPixelY totalHeight() const;

  public: // services
    /** gives the index of the line that would include y in pixel coord.
      * y is not forced to be inside the total height.
      */
    uint lineAt( KPixelY y ) const;
    /** gives the index of the first and the last line that would be visible
      * these lines might not contain anything
      */
    KSection visibleLines() const;
    /** gives the index of the first and the last line that would be visible in the given pixel range
      * these lines might not contain anything
      */
    KSection visibleLines( KPixelYs YPixels ) const;


  protected:
    /** called by KColumn */
    void addColumn( KColumn *C );
    void removeColumn( KColumn *C );


  protected: //
    /** sets height of all lines and propagates this information to all columns
      * doesn't update the content size
      * @param NewLineHeight height in pixels
      */
    virtual void setLineHeight( KPixelY NewLineHeight );
    /** sets the number of lines
      * doesn't update the content size
      * @param NewNoOfLines new number of lines to display
      */
    virtual void setNoOfLines( int NewNoOfLines );

  protected: // recalculations
    /** recalculates the positions of the columns and the total width */
    void updateWidths();
    /** ensures that the line buffer has the size of the whole line */
    void updateLineBufferSize();

  protected: // painting
    void updateView();
    void repaintView();

  private:
    /** hiding it*/
    void drawContents( QPainter* );


  protected: // calculated
    /** collection of all the columns. All columns will be autodeleted. */
    QPtrList<KColumn> Columns;
    /** the number of lines which the column view has */
    int NoOfLines;
    /** the height of each line in pixels */
    KPixelY LineHeight;
    /** the width of all visible columns together */
    KPixelX TotalWidth;
//    /** width that is used by columns that are not resizeable (if shown) */
//    KPixelX ReservedWidth;

  protected:
    // TODO: do we really want this?
    bool HorizontalGrid;
    /** Buffer where each line is drawn to before it is copied onto the screen */
    QPixmap  LineBuffer;
//    bool Reversed;

  private:
    KColumnsViewPrivate *d;
};


inline int KColumnsView::noOfLines()          const { return NoOfLines; }
inline KPixelY KColumnsView::lineHeight()     const { return LineHeight; }
inline uint KColumnsView::lineAt( KPixelY y ) const { return LineHeight!=0 ? y / LineHeight : 0; }
inline KSection KColumnsView::visibleLines() const
{
  KPixelY cy = contentsY();
  KPixelY ch = visibleHeight();
  return KSection( lineAt(cy), lineAt(cy+ch-1) );
}

inline KSection KColumnsView::visibleLines( KPixelYs YPixels ) const
{
  return KSection( lineAt(YPixels.start()), lineAt(YPixels.end()) );
}

inline KPixelY KColumnsView::totalHeight()   const { return NoOfLines*LineHeight; }
inline KPixelX KColumnsView::totalWidth()    const { return TotalWidth; }

inline QSize KColumnsView::totalViewSize()   const { return QSize( totalWidth(), totalHeight() ); }

}

#endif
