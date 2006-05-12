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


#include <kdebug.h>

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


void KColumnsView::paintEmptyArea( QPainter *Painter, int cx ,int cy, int cw, int ch)
{
    Painter->fillRect( cx, cy, cw, ch, palette().brush(backgroundRole()) );
}


void KColumnsView::drawContents( QPainter *Painter, int cx, int cy, int cw, int ch )
{
  //kDebug(1501) << "drawContents(" << cx<<","<<cw<<"#"<<cy<<","<<ch<<")\n";
  KPixelXs DirtyXs( cx, cw, true );

  // content to be shown?
  if( DirtyXs.startsBefore(TotalWidth) )
  {
    KPixelYs DirtyYs( cy, ch, true );

    // collect affected columns
    QList<KColumn*> DirtyColumns;
    QListIterator<KColumn*> it( Columns );
    while( it.hasNext() )
    {
      KColumn *Column = it.next();
      if( Column->isVisible() && Column->overlaps(DirtyXs) )
        DirtyColumns.append( Column );
    }

    // any lines of any columns to be drawn?
    if( NoOfLines > 0 )
    {
      // calculate affected lines
      KSection DirtyLines = visibleLines( DirtyYs );
      DirtyLines.restrictEndTo( NoOfLines - 1 );

      if( DirtyLines.isValid() )
      {
        KPixelY cy = DirtyLines.start() * LineHeight;

        // starting painting with the first line
        int Line = DirtyLines.start();
        QListIterator<KColumn*> it( DirtyColumns );
        KColumn *Column = it.next();
        Painter->translate( Column->x(), cy );

        while( true )
        {
          Column->paintFirstLine( Painter, DirtyXs, Line );
          if( !it.hasNext() )
            break;
          Painter->translate( Column->width(), 0 );
          Column = it.next();
        }
        Painter->translate( -Column->x(), 0 );

        // Go through the other lines
        while( true )
        {
          ++Line;

          if( Line > DirtyLines.end() )
            break;

          QListIterator<KColumn*> it( DirtyColumns );
          Column = it.next();
          Painter->translate( Column->x(), LineHeight );

          while( true )
          {
            Column->paintNextLine( Painter );
            if( !it.hasNext() )
              break;
            Painter->translate( Column->width(), 0 );
            Column = it.next();
          }
          Painter->translate( -Column->x(), 0 );
        }
        cy = DirtyLines.end() * LineHeight;

        Painter->translate( 0, -cy );
      }
    }

    // draw empty columns?
    DirtyYs.setStart( totalHeight() );
    if( DirtyYs.isValid() )
    {
      QListIterator<KColumn*> it( DirtyColumns );
      while( it.hasNext() )
        it.next()->paintEmptyColumn( Painter, DirtyXs, DirtyYs );
    }
  }

  // Painter empty rects
  DirtyXs.setStart( TotalWidth );
  if( DirtyXs.isValid() )
    paintEmptyArea( Painter, DirtyXs.start(), cy, DirtyXs.width(), ch );
}

#include "kcolumnsview.moc"
