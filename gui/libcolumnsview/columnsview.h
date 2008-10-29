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

#ifndef KHE_UI_COLUMNSVIEW_H
#define KHE_UI_COLUMNSVIEW_H


// lib
#include "kadds.h"
#include "oktetagui_export.h"
// commonlib
#include <ksection.h> // TODO: think about moving this out of the public API
// Qt
#include <QtCore/QList>
#include <QtGui/QAbstractScrollArea>


namespace KHEUI
{

class ColumnRenderer;
class ColumnsViewPrivate;

/** general class for widgets with columns that display different aspects of the same data
  * with the same lineheight for all lines
  *
  *@author Friedrich W. H. Kossebau
  */

class OKTETAGUI_EXPORT ColumnsView : public QAbstractScrollArea
{
   Q_OBJECT

   friend class ColumnRenderer;

  public:
    explicit ColumnsView( /*bool R = false,*/ QWidget *parent = 0 );
    virtual ~ColumnsView();

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
    QPoint viewportToColumns( const QPoint &point ) const;

  public:
    /**  */
    void setColumnsPos( KPixelX x, KPixelY y );

  protected: // QAbstractScrollArea API
    virtual bool event( QEvent *event );
    virtual void resizeEvent( QResizeEvent *event );
    virtual void paintEvent( QPaintEvent *paintEvent );
    virtual void scrollContentsBy( int dx, int dy );

  protected: // our API
    /** draws all columns in columns coordinates */
    virtual void renderColumns( QPainter *painter, int cx, int cy, int cw, int ch );
    /** draws area without columns in columns coordinates */
    virtual void renderEmptyArea( QPainter *painter, int cx, int cy, int cw, int ch );

  protected:
    /** called by ColumnRenderer */
    void addColumn( ColumnRenderer *columnRenderer );
    void removeColumn( ColumnRenderer *columnRenderer );

  protected: //
    /** sets height of all lines and propagates this information to all columns
      * doesn't update the content size
      * @param NewLineHeight height in pixels
      */
    virtual void setLineHeight( KPixelY lineHeight );
    /** sets the number of lines
      * doesn't update the content size
      * @param NewNoOfLines new number of lines to display
      */
    virtual void setNoOfLines( int noOfLines );

  protected: // recalculations
    /** recalculates the positions of the columns and the total width */
    void updateWidths();
    void updateScrollBars();
    /** calls updateContent for the Column */
    void updateColumn( ColumnRenderer& columnRenderer );

  private:
    ColumnsViewPrivate * const d;
};

}

#endif
