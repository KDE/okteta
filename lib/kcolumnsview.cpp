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


//#include <kdebug.h>

// qt specific
#include <qpainter.h>
// lib specific
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
  P->fillRect( cx, cy, cw, ch, backgroundBrush() );
}


void KColumnsView::drawContents( QPainter *P, int cx, int cy, int cw, int ch )
{
  //kdDebug(1501) << "drawContents(" << cx<<","<<cw<<"#"<<cy<<","<<ch<<")\n";
  KPixelXs AffectedXs( cx, cw, true );
  // content to be shown?
  if( AffectedXs.startsBefore(TotalWidth) )
  {
    KPixelYs AffectedYs( cy, ch, true );

    // collect affected columns
    QPtrList<KColumn> RedrawColumns;
    for( KColumn *C=Columns.first(); C; C=Columns.next() )
      if( C->isVisible() && C->overlaps(AffectedXs) )
        RedrawColumns.append( C );

    // any lines to be drawn?
    if( NoOfLines > 0 )
    {
      // calculate affected lines
      KSection AffectedLines = visibleLines( AffectedYs );
      AffectedLines.restrictEndTo( NoOfLines - 1 );

      if( AffectedLines.isValid() )
      {
        QPainter Paint;
        Paint.begin( &LineBuffer, this );

        // starting painting with the first line
        KColumn *C = RedrawColumns.first();
        Paint.translate( C->x(), 0 );

        for( ; C; C=RedrawColumns.next() )
        {
          C->paintFirstLine( &Paint, AffectedXs, AffectedLines.start() );
          Paint.translate( C->width(), 0 );
        }

        // Go through the other lines
        KPixelY y = AffectedLines.start() * LineHeight;
        int l = AffectedLines.start();
        while( true )
        {
          Paint.end();
          P->drawPixmap( cx, y, LineBuffer, cx, 0, cw, LineHeight ); // bitBlt directly impossible: lack of real coord

          // copy to screen
//        bitBlt( viewport(), cx - contentsX(), y - contentsY(),
//                &LineBuffer, cx, 0, cw, LineHeight );

          ++l;
          y += LineHeight;

          if( l > AffectedLines.end() )
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

    // draw empty columns?
    AffectedYs.setStart( totalHeight() );
    if( AffectedYs.isValid() )
    {
      for( KColumn *C = RedrawColumns.first(); C; C=RedrawColumns.next() )
        C->paintEmptyColumn( P, AffectedXs, AffectedYs );
    }
  }

  // Paint empty rects
  AffectedXs.setStart( TotalWidth );
  if( AffectedXs.isValid() )
    paintEmptyArea( P, AffectedXs.start(), cy, AffectedXs.width(), ch );
}

// Implemented to get rid of a compiler warning
void KColumnsView::drawContents( QPainter * ) {}

#include "kcolumnsview.moc"
