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

 
#ifndef KCOLUMNSVIEW_H
#define KCOLUMNSVIEW_H

#include <qptrlist.h>
#include <qwidget.h>
#include <qpixmap.h>
#include <qscrollview.h>

#include "kadds.h"


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

  public:
    /** sets height of all lines and propagates this information to all columns
      * @param NewLineHeight height in pixels
      */
    void setLineHeight( KPixelY NewLineHeight );

  public: // data-wise sizes
    /** returns the number of all lines */
    int noOfLines() const;
    /** returns number of full visible Lines, at least 1 (as needed by page down/up)*/
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

  public:
    /** gives the index of the line that would include y in pixel coord.
      * y needs not be inside the total height.
      */
    int lineAt( KPixelY y ) const;


  protected:
    /** called by KColumn */
    void addColumn( KColumn *C );
    void removeColumn( KColumn *C );


  protected:
    /** recalculates the positions of the columns and the total width */
    void updateWidths();
    /** ensures that the line buffer has the size of the whole line */
    void updateLineBufferSize();
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


inline KPixelY KColumnsView::lineHeight()    const { return LineHeight; }
inline int KColumnsView::lineAt( KPixelY y ) const { return LineHeight ? y / LineHeight : -1; }

inline KPixelY KColumnsView::totalHeight()   const { return NoOfLines*LineHeight; }
inline KPixelX KColumnsView::totalWidth()    const { return TotalWidth; }

inline QSize KColumnsView::totalViewSize()   const { return QSize( totalWidth(), totalHeight() ); }

}

#endif
