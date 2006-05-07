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
#include <QListIterator>
#include <QPainter>
// lib specific
#include "kcolumn.h"
#include "kcolumnsview.h"


using namespace KHE;

static bool DefaultHorizontalGrid = false;

KColumnsView::KColumnsView( /*bool R,*/ QWidget *Parent, const char *Name, Qt::WFlags Flags )
 : Q3ScrollView( Parent, Name, Flags | Qt::WNoAutoErase /*| WStaticContents*/ ),
   NoOfLines( 0 ),
   LineHeight( 0 ),
   TotalWidth( 0 ),
   HorizontalGrid( DefaultHorizontalGrid ),
//    Reversed( R ),
   d( 0 )
{
  viewport()->setBackgroundRole ( QPalette::Base );
  setBackgroundRole( QPalette::Background );
  viewport()->setFocusProxy( this );
}

KColumnsView::~KColumnsView()
{
  while( !Columns.isEmpty() )
    delete Columns.takeFirst();
}


void KColumnsView::setNoOfLines( int NewNoOfLines )
{
  NoOfLines = NewNoOfLines;
}


void KColumnsView::setLineHeight( KPixelY LH )
{
  LineHeight = LH;
  QListIterator<KColumn*> it( Columns );
  while( it.hasNext() )
    it.next()->setLineHeight( LineHeight );
  verticalScrollBar()->setSingleStep( LineHeight );

  updateLineBufferSize();
}


void KColumnsView::updateWidths()
{
  TotalWidth = 0;
  QListIterator<KColumn*> it( Columns );
  while( it.hasNext() )
  {
    KColumn *Column = it.next();
    Column->setX( TotalWidth );
    TotalWidth += Column->visibleWidth();
  }

  updateLineBufferSize();
}


void KColumnsView::updateLineBufferSize()
{
  int w = totalWidth();
  int h = LineHeight;

  if( w != LineBuffer.width() || h != LineBuffer.height() )
    LineBuffer = QPixmap( w, h );
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
  int Pos = Columns.indexOf( C );
  if( Pos != -1 )
    Columns.removeAt( Pos );

  delete C;

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
    P->fillRect( cx, cy, cw, ch, palette().brush(backgroundRole()) );
}


void KColumnsView::drawContents( QPainter *P, int cx, int cy, int cw, int ch )
{
  //kDebug(1501) << "drawContents(" << cx<<","<<cw<<"#"<<cy<<","<<ch<<")\n";
  KPixelXs AffectedXs( cx, cw, true );
  // content to be shown?
  if( AffectedXs.startsBefore(TotalWidth) )
  {
    KPixelYs AffectedYs( cy, ch, true );

    // collect affected columns
    QList<KColumn*> RedrawColumns;
    QListIterator<KColumn*> it( Columns );
    while( it.hasNext() )
    {
      KColumn *Column = it.next();
      if( Column->isVisible() && Column->overlaps(AffectedXs) )
        RedrawColumns.append( Column );
    }

    // any lines to be drawn?
    if( NoOfLines > 0 )
    {
      // calculate affected lines
      KSection AffectedLines = visibleLines( AffectedYs );
      AffectedLines.restrictEndTo( NoOfLines - 1 );

      if( AffectedLines.isValid() )
      {
        QPainter Paint;
        Paint.begin( &LineBuffer );
        Paint.initFrom( this );

        // starting painting with the first line
        QListIterator<KColumn*> it( RedrawColumns );
        if( it.hasNext() )
          Paint.translate( it.next()->x(), 0 );

        while( it.hasNext() )
        {
          KColumn *Column = it.next();
          Column->paintFirstLine( &Paint, AffectedXs, AffectedLines.start() );
          Paint.translate( Column->width(), 0 );
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
          Paint.begin( &LineBuffer );
          Paint.initFrom( this );

          QListIterator<KColumn*> it( RedrawColumns );
          if( it.hasNext() )
            Paint.translate( it.next()->x(), 0 );

          while( it.hasNext() )
          {
            KColumn *Column = it.next();
            Column->paintNextLine( &Paint );
            Paint.translate( Column->width(), 0 );
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
      QListIterator<KColumn*> it( RedrawColumns );
      while( it.hasNext() )
        it.next()->paintEmptyColumn( P, AffectedXs, AffectedYs );
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
