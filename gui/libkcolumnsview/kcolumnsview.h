/***************************************************************************
                          kcolumnsview.h  -  description
                             -------------------
    begin                : Mit Mai 21 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_UI_KCOLUMNSVIEW_H
#define KHE_UI_KCOLUMNSVIEW_H


// lib
#include "kadds.h"
// commonlib
#include <khe_export.h>
#include <ksection.h> // TODO: think about moving this out of the public API
// Qt
#include <QtCore/QList>
#include <QtGui/QAbstractScrollArea>


namespace KHEUI
{

class KColumn;
class KColumnsViewPrivate;

/** general class for widgets with columns that display different aspects of the same data
  * with the same lineheight for all lines
  *
  *@author Friedrich W. H. Kossebau
  */

class KHEUI_EXPORT KColumnsView : public QAbstractScrollArea
{
   Q_OBJECT

   friend class KColumn;

  public:
    explicit KColumnsView( /*bool R = false,*/ QWidget *Parent = 0 );
    virtual ~KColumnsView();

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
    /** returns the width of all visible columns together */
    KPixelX columnsWidth() const;
    /** returns the height of all lines together */
    KPixelY columnsHeight() const;

  public: // services
    /** gives the index of the line that would include y in pixel coord.
      * y is not forced to be inside the total height.
      */
    uint lineAt( KPixelY y ) const;
    /** gives the index of the first and the last line that would be visible
      * these lines might not contain anything
      */
    KHE::KSection visibleLines() const;
    /** gives the index of the first and the last line that would be visible in the given pixel range
      * these lines might not contain anything
      */
    KHE::KSection visibleLines( const KPixelYs &YPixels ) const;

    /** @return visible width of the current view */
    KPixelX visibleWidth() const;
    /** @return visible height of the current view */
    KPixelY visibleHeight() const;
    /** @return x offset of the current view */
    KPixelX xOffset() const;
    /** @return y offset of the current view */
    KPixelY yOffset() const;

    /** translates the point to coordinates in the columns */
    QPoint viewportToColumns( const QPoint &P ) const;

  public:
    /**  */
    void setColumnsPos( KPixelX x, KPixelY y );

  protected: // QAbstractScrollArea API
    virtual bool event( QEvent *Event );
    virtual void resizeEvent( QResizeEvent *ResizeEvent );
    virtual void paintEvent( QPaintEvent *Event );
    virtual void scrollContentsBy( int dx, int dy );

  protected: // our API
    /** draws all columns in columns coordinates */
    virtual void drawColumns( QPainter *p, int cx, int cy, int cw, int ch );
    /** draws area without columns in columns coordinates */
    virtual void drawEmptyArea( QPainter *p, int cx, int cy, int cw, int ch );

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
    void updateScrollBars();

  private:
    KColumnsViewPrivate * const d;
};

}

#endif
