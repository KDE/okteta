/***************************************************************************
                          kcolumnsview.cpp  -  description
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


// #include <iostream>

#include <qpainter.h>

#include "kcolumn.h"
#include "kcolumnsview.h"


using namespace KHE;

static bool DefaultHorizontalGrid = false;

KColumnsView::KColumnsView( /*bool R,*/ QWidget *Parent, const char *Name, WFlags Flags )
 : QScrollView( Parent, Name, Flags | WRepaintNoErase /*| WStaticContents*/ ),
   NoOfLines( 0 ),
   LineHeight( 0 ),
   TotalWidth( 0 ),
   HorizontalGrid( DefaultHorizontalGrid ),
//    Reversed( R ),
   d( 0 )
{
  viewport()->setBackgroundMode( PaletteBase );
  setBackgroundMode( PaletteBackground, PaletteBase );
  viewport()->setFocusProxy( this );

  Columns.setAutoDelete( true );
}

KColumnsView::~KColumnsView()
{
}


void KColumnsView::setNoOfLines( int NewNoOfLines )
{
  NoOfLines = NewNoOfLines;
}


void KColumnsView::setLineHeight( KPixelY LH )
{
  LineHeight = LH;
  for( KColumn *C=Columns.first(); C; C=Columns.next() )
    C->setLineHeight( LineHeight );
  verticalScrollBar()->setLineStep( LineHeight );

  updateLineBufferSize();
}


void KColumnsView::updateWidths()
{
  TotalWidth = 0;
  for( KColumn *C=Columns.first(); C; C=Columns.next() )
  {
    C->setX( TotalWidth );
    TotalWidth += C->visibleWidth();
  }

  updateLineBufferSize();
}


void KColumnsView::updateLineBufferSize()
{
  int w = totalWidth();
  int h = LineHeight;

  if( w != LineBuffer.width() || h != LineBuffer.height() )
    LineBuffer.resize( w, h );
}


int KColumnsView::noOfLinesPerPage() const
{
  if( !viewport() || LineHeight == 0 )
    return 1;
//  int NoOfLinesPerPage = (visibleHeight()-1) / LineHeight; // -1 ensures to get always the last visible line
  int NoOfLinesPerPage = (viewport()->height()-1) / LineHeight; // -1 ensures to get always the last visible line

  if( NoOfLinesPerPage == 0 )
    // ensure to move down at least one line
    NoOfLinesPerPage = 1;
  return NoOfLinesPerPage;
}


void KColumnsView::addColumn( KColumn *C )
{
//   if( Reversed )
//     Columns.prepend( C );
//   else
    Columns.append( C );

  updateWidths();
}


void KColumnsView::removeColumn( KColumn *C )
{
  Columns.remove( C );

  updateWidths();
}


void KColumnsView::updateView()
{
  resizeContents( totalWidth(), totalHeight() );
  updateContents();
}


void KColumnsView::repaintView()
{
  resizeContents( totalWidth(), totalHeight() );
  repaintContents( false );
}


void KColumnsView::paintEmptyArea( QPainter *P, int cx ,int cy, int cw, int ch)
{
  // is the viewport (content) totally filled?
  if( totalWidth() >= viewport()->width() && totalHeight() >= viewport()->height() )
    return;

  // Region of the rect we should draw
  contentsToViewport( cx, cy, cx, cy );
  QRegion Region( QRect(cx,cy,cw,ch) );
  // Subtract all the columns from it
  Region = Region.subtract( QRect(contentsToViewport( QPoint(0,0) ),totalViewSize()) );

  // finally draw the rectangles (transformed as needed)
  QMemArray<QRect> Rectangles = Region.rects();
  const QBrush &Brush = backgroundBrush();
  for( int i=0; i<(int)Rectangles.count(); ++i ) 
    P->fillRect( Rectangles[i], Brush );
}
 

void KColumnsView::drawContents( QPainter *P, int cx, int cy, int cw, int ch )
{
  // calculate affected lines
  int FirstLine = lineAt( cy );

  // content to be shown?
  if( NoOfLines > 0 && FirstLine != -1 && cx < TotalWidth )
  {
    // correct bounds
    int LastLine = lineAt( cy + ch - 1 );
    if( LastLine < 0 || LastLine >= NoOfLines )
      LastLine = NoOfLines - 1;

    // collect affected columns
    QPtrList<KColumn> RedrawColumns;
    for( KColumn *C=Columns.first(); C; C=Columns.next() )
      if( C->isVisible() && C->overlaps(cx,cx+cw-1) )
        RedrawColumns.append( C );

    if( !RedrawColumns.isEmpty() )
    {
      QPainter Paint;
      Paint.begin( &LineBuffer, this );

      // starting painting with the first line
      KColumn *C = RedrawColumns.first();
      Paint.translate( C->x(), 0 );

      for( ; C; C=RedrawColumns.next() )
      {
        C->paintFirstLine( &Paint, cx, cw, FirstLine );
        Paint.translate( C->width(), 0 );
      }

      // Go through the other lines
      KPixelY y = FirstLine*LineHeight;
      int l = FirstLine;
      while( true )
      {
        Paint.end();
         P->drawPixmap( cx, y, LineBuffer, cx, 0, cw, LineHeight ); // bitBlt directly impossible: lack of real coord

        // copy to screen
//        bitBlt( viewport(), cx - contentsX(), y - contentsY(),
//                &LineBuffer, cx, 0, cw, LineHeight );

        ++l;
        y += LineHeight;

        if( l > LastLine )
          break;

        // to avoid flickers we first paint to the linebuffer
        Paint.begin( &LineBuffer, this );

        KColumn *C = RedrawColumns.first();
        Paint.translate( C->x(), 0 );

        for( ; C; C=RedrawColumns.next() )
        {
          C->paintNextLine( &Paint );
          Paint.translate( C->width(), 0 );
        }

        if( HorizontalGrid && cx < TotalWidth )
          Paint.drawLine( cx, LineHeight-1, TotalWidth-1, LineHeight-1 );  // TODO: use a additional TotalHeight?
      }
    }
  }

  // Paint empty rects
  paintEmptyArea( P, cx, cy, cw, ch );
}

// Implemented to get rid of a compiler warning
void KColumnsView::drawContents( QPainter * ) {}

#include "kcolumnsview.moc"
