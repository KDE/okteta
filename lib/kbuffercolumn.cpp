/***************************************************************************
                          kbuffercolumn.cpp  -  description
                             -------------------
    begin                : Mit Mai 14 2003
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
#include "kcolumnsview.h"
#include "kbuffercursor.h"
#include "kbuffercolumn.h"
#include "kbufferlayout.h"
#include "kbufferranges.h"
#include "helper.h"
#include "kcharcodec.h"

using namespace KHE;

static const unsigned int StartsBefore = 1;
static const unsigned int EndsLater = 2;
static const char EmptyByte = ' ';

static const KPixelX DefaultCursorWidth = 2;
static const KPixelX DefaultByteSpacingWidth = 3;
static const KPixelX DefaultGroupSpacingWidth = 9;
static const int DefaultNoOfGroupedBytes = 4;

KBufferColumn::KBufferColumn( KColumnsView *CV, KDataBuffer *B, KBufferLayout *L, KBufferRanges *R )
 : KColumn( CV ),
   Buffer( B ),
   Layout( L ),
   Ranges( R ),
   DigitWidth( 0 ),
   DigitBaseLine( 0 ),
   VerticalGrid( false ),
   ByteWidth( 0 ),
   ByteSpacingWidth( DefaultByteSpacingWidth ),
   GroupSpacingWidth( DefaultGroupSpacingWidth ),
   NoOfGroupedBytes( DefaultNoOfGroupedBytes ),
   PosX( 0L ),
   PosRightX( 0L ),
   LastPos( 0 )
{
}


KBufferColumn::~KBufferColumn()
{
  delete [] PosX;
  delete [] PosRightX;
}



void KBufferColumn::set( KDataBuffer *B )
{
  Buffer= B;
}


void KBufferColumn::resetXBuffer()
{
  delete [] PosX;
  delete [] PosRightX;

  LastPos = Layout->noOfBytesPerLine()-1;
  PosX =      new KPixelX[LastPos+1];
  PosRightX = new KPixelX[LastPos+1];

  if( PosX )
    recalcX();
}


void KBufferColumn::setMetrics( KPixelX DW, KPixelY DBL )
{
  DigitBaseLine = DBL;
  setDigitWidth( DW );
}


bool KBufferColumn::setDigitWidth( KPixelX DW )
{
  // no changes?
  if( DigitWidth == DW )
    return false;

  DigitWidth = DW;
  // recalculate depend sizes
  recalcByteWidth();

  if( PosX )
    recalcX();
  return true;
}


bool KBufferColumn::setSpacing( KPixelX BSW, int NoGB, KPixelX GSW )
{
  // no changes?
  if( ByteSpacingWidth == BSW && NoOfGroupedBytes == NoGB && GroupSpacingWidth == GSW )
    return false;

  ByteSpacingWidth = BSW;
  NoOfGroupedBytes = NoGB;
  GroupSpacingWidth = GSW;

  // recalculate depend sizes
  recalcVerticalGridX();

  if( PosX )
    recalcX();
  return true;
}


bool KBufferColumn::setByteSpacingWidth( KPixelX BSW )
{
  // no changes?
  if( ByteSpacingWidth == BSW )
    return false;

  ByteSpacingWidth = BSW;

  // recalculate depend sizes
  recalcVerticalGridX();

  if( PosX )
    recalcX();
  return true;
}


bool KBufferColumn::setNoOfGroupedBytes( int NoGB )
{
  // no changes?
  if( NoOfGroupedBytes == NoGB )
    return false;

  NoOfGroupedBytes = NoGB;

  if( PosX )
    recalcX();
  return true;
}


bool KBufferColumn::setGroupSpacingWidth( KPixelX GSW )
{
  // no changes?
  if( GroupSpacingWidth == GSW )
    return false;

  GroupSpacingWidth = GSW;

  // recalculate depend sizes
  recalcVerticalGridX();

  if( PosX )
    recalcX();
  return true;
}


void KBufferColumn::recalcByteWidth()
{
  ByteWidth = DigitWidth;
  recalcVerticalGridX();
}


void KBufferColumn::recalcVerticalGridX()
{
  VerticalGridX = ByteWidth-1 + GroupSpacingWidth/2;
}


void KBufferColumn::recalcX()
{
  SpacingTrigger = noOfGroupedBytes() > 0 ? noOfGroupedBytes()-1 : LastPos+1; // last ensures to never trigger the spacing

  KPixelX NewWidth = 0;
  int p = 0;
  int gs = 0;
  KPixelX *PX = PosX;
  KPixelX *PRX = PosRightX;
  for( ; PX<&PosX[LastPos+1]; ++PX, ++PRX, ++p, ++gs )
  {
    *PX = NewWidth;
    NewWidth += ByteWidth;
    *PRX = NewWidth-1;

    // is there a space behind the actual byte (if it is not the last)?
    if( gs == SpacingTrigger )
    {
      NewWidth += GroupSpacingWidth;
      gs = -1;
    }
    else
      NewWidth += ByteSpacingWidth;
  }
  setWidth( PosRightX[LastPos]+1 );
}


// TODO: why are inlined functions not available as symbols when defined before their use
//TODO: works not precisly for the byte rects but includes spacing and left and right
/*inline*/ int KBufferColumn::posOfX( KPixelX PX ) const
{
  if( !PosX )
    return NoByteFound;

  // translate
  PX -= x();
  // search backwards for the first byte that is equalleft to x
  for( int p=LastPos; p>=0; --p )
    if( PosX[p] <= PX )
      return p;

  return 0; //NoByteFound;
}


int KBufferColumn::magPosOfX( KPixelX PX ) const
{
  if( !PosX )
    return NoByteFound;

  // translate
  PX -= x();
  // search backwards for the first byte that is equalleft to x
  for( int p=LastPos; p>=0; --p )
    if( PosX[p] <= PX )
    {
      // are we close to the right?
      if( PosRightX[p]-PX < DigitWidth/2 ) // TODO: perhaps cache also the middle xpos's
        ++p;
      return p;
    }

  return 0; //NoByteFound;
}


KSection KBufferColumn::posOfX( KPixelX PX, KPixelX PW ) const
{
  if( !PosX )
    return KSection();

  // translate
  PX -= x();
  int PRX = PX + PW - 1;

  KSection P;
  // search backwards for the first byte that is equalleft to x
  for( int p=LastPos; p>=0; --p )
    if( PosX[p] <= PRX )
    {
      P.setEnd( p );
      for( ; p>=0; --p )
        if( PosX[p] <= PX )
        {
          P.setStart( p );
          break;
        }
      break;
    }

  return P;
}


KPixelX KBufferColumn::xOfPos( int Pos )      const { return x() + (PosX?PosX[Pos]:0); }
KPixelX KBufferColumn::rightXOfPos( int Pos ) const { return x() + (PosRightX?PosRightX[Pos]:0); }


int KBufferColumn::posOfRelX( KPixelX PX ) const
{
  if( !PosX )
    return NoByteFound;

  // search backwards for the first byte that is equalleft to x
  for( int p=LastPos; p>=0; --p )
    if( PosX[p] <= PX )
      return p;

  return 0; //NoByteFound;
}


KSection KBufferColumn::posOfRelX( KPixelX PX, KPixelX PW ) const
{
  if( !PosX )
    return KSection();

  int PRX = PX + PW - 1;

  KSection P;
  // search backwards for the first byte that is equalleft to x
  for( int p=LastPos; p>=0; --p )
    if( PosX[p] <= PRX )
    {
      P.setEnd( p );
      for( ; p>=0; --p )
        if( PosX[p] <= PX )
        {
          P.setStart( p );
          break;
        }
      break;
    }

  return P;
}


KPixelX KBufferColumn::relXOfPos( int Pos )      const { return PosX ? PosX[Pos] : 0; }
KPixelX KBufferColumn::relRightXOfPos( int Pos ) const { return PosRightX ? PosRightX[Pos] : 0; }


KPixelXs KBufferColumn::wideXPixelsOfPos( KSection Positions ) const
{
  return KPixelXs( Positions.start()>0?rightXOfPos(Positions.start()-1)+1:xOfPos(Positions.start()),
                   Positions.end()<LastPos?xOfPos(Positions.end()+1)-1:rightXOfPos(Positions.end())  );
}


KPixelXs KBufferColumn::relWideXPixelsOfPos( KSection Positions ) const
{
  return KPixelXs( Positions.start()>0?relRightXOfPos(Positions.start()-1)+1:relXOfPos(Positions.start()),
                   Positions.end()<LastPos?relXOfPos(Positions.end()+1)-1:relRightXOfPos(Positions.end())  );
}


void KBufferColumn::preparePainting( KPixelXs Xs )
{
  Xs.restrictTo( XSpan );
  // translate
  Xs.moveBy( -x() );


  // store the values
  PaintX = Xs.start();
  PaintW = Xs.width();

  // get line positions to paint
  PaintPositions = posOfRelX( PaintX, PaintW );
}


void KBufferColumn::paintFirstLine( QPainter *P, KPixelXs Xs, int FirstLine )
{
  preparePainting( Xs );

  PaintLine = FirstLine;

//  kdDebug(1501) << "paintFirstLine:"<<cx<<","<<cw<<"|" <<PaintX<<","<<PaintW << ")\n";

//  paintPositions( P, PaintLine++, PaintPositions );
  paintLine( P, PaintLine++ );
}


void KBufferColumn::paintNextLine( QPainter *P )
{
//  paintPositions( P, PaintLine++, PaintPositions );
  paintLine( P, PaintLine++ );
}


void KBufferColumn::paintLine( QPainter *P, int Line ) // TODO: could be removed???
{
//  kdDebug(1501) << "paintLine line: "<<Line<<" Start: "<<PaintPositions.start()<<" End: "<<PaintPositions.end() << "\n";
  // no bytes to paint?
//   if( !Layout->hasContent(Line) )
//     return;

  paintPositions( P, Line, PaintPositions );
}


void KBufferColumn::paintPositions( QPainter *P, int Line, KSection Pos )
{
  const QColorGroup &CG = View->colorGroup();

  // clear background
  unsigned int BlankFlag = (Pos.start()!=0?StartsBefore:0) | (Pos.end()!=LastPos?EndsLater:0);
  paintRange( P, CG.base(), Pos, BlankFlag );

  // Go through the lines TODO: handle first and last line more effeciently
  // check for leading and trailing spaces
  KSection Positions( Layout->firstPos(KBufferCoord( Pos.start(), Line )),
                      Layout->lastPos( KBufferCoord( Pos.end(),  Line )) );

  // no bytes to paint?
  if( !Layout->hasContent(Line) )
    return;

  // check for leading and trailing spaces
  KSection Indizes( Layout->indexAtCoord(KBufferCoord( Positions.start(), Line )), Positions.width(), false );

  unsigned int SelectionFlag;
  unsigned int MarkingFlag;
  KSection Selection;
  KSection Marking;
  bool HasMarking = Ranges->hasMarking();
  bool HasSelection = Ranges->hasSelection();

  while( Positions.isValid() )
  {
    KSection PositionsPart( Positions );  // set of positions to paint next
    KSection IndizesPart( Indizes );      // set of indizes to paint next
    // falls Marking nicht mehr gebuffert und noch zu erwarten
    if( HasMarking && Marking.endsBefore(IndizesPart.start()) )
    {
      // erhebe nächste Markierung im Bereich
      HasMarking = isMarked( IndizesPart, &Marking, &MarkingFlag );
    }
    // falls Selection nicht mehr gebuffert und noch zu erwarten
    if( HasSelection && Selection.endsBefore(IndizesPart.start()) )
    {
      // erhebe nächste Selection im Bereich
      HasSelection = isSelected( IndizesPart, &Selection, &SelectionFlag );
    }

    if( Marking.start() == IndizesPart.start() )
    {
      IndizesPart.setEnd( Marking.end() );
      PositionsPart.setEndByWidth( Marking.width() );
      if( PositionsPart.end() == Layout->lastPos(Line) )   MarkingFlag &= ~EndsLater;
      if( PositionsPart.start() == Layout->firstPos(Line)) MarkingFlag &= ~StartsBefore;
      paintMarking( P, PositionsPart, IndizesPart.start(), MarkingFlag );

    }
    else if( Selection.includes(IndizesPart.start()) )    
    {
      if( Selection.startsBehind(IndizesPart.start()) )
        SelectionFlag |= StartsBefore;
      bool MarkingBeforeEnd = HasMarking && Marking.start() <= Selection.end();

      IndizesPart.setEnd( MarkingBeforeEnd ? Marking.start()-1 : Selection.end() );
      PositionsPart.setEndByWidth( IndizesPart.width() );

      if( MarkingBeforeEnd )
        SelectionFlag |= EndsLater;
      if( PositionsPart.end() == Layout->lastPos(Line) )    SelectionFlag &= ~EndsLater;
      if( PositionsPart.start() == Layout->firstPos(Line) ) SelectionFlag &= ~StartsBefore;

      paintSelection( P, PositionsPart, IndizesPart.start(), SelectionFlag );
    }
    else
    {
      // calc end of plain text
      if( HasMarking )
        IndizesPart.setEnd( Marking.start()-1 );
      if( HasSelection )
        IndizesPart.restrictEndTo( Selection.start()-1 );

      PositionsPart.setEndByWidth( IndizesPart.width() );
      paintPlain( P, PositionsPart, IndizesPart.start() );
    }
    Indizes.setStartBehind( IndizesPart );
    Positions.setStartBehind( PositionsPart );
  }
  // we don't paint grids as default, perhaps we never will though it works
  // paintGrid( P, Positions ); // TODO: get some unmodified Positions (see three lines before)
}


void KBufferColumn::paintPlain( QPainter *P, KSection Positions, int Index )
{
  // paint all the bytes affected
  for( int p=Positions.start(); p<=Positions.end(); ++p,++Index )
  {
    KPixelX x = relXOfPos( p );

    // draw the byte
    P->translate( x, 0 );
    char Byte = Buffer->datum( Index );
    KHEChar B = Codec->decode( Byte );
    drawByte( P, Byte, B, colorForChar(B) );

    P->translate( -x, 0 );
  }
}


void KBufferColumn::paintSelection( QPainter *P, KSection Positions, int Index, int Flag )
{
  const QColorGroup &CG = View->colorGroup();

  paintRange( P, CG.highlight(), Positions, Flag );

  const QColor &HTC = CG.highlightedText();
  // paint all the bytes affected
  for( int p=Positions.start(); p<=Positions.end(); ++p,++Index )
  {
    KPixelX x = relXOfPos( p );

    // draw the byte
    P->translate( x, 0 );
    char Byte = Buffer->datum( Index );
    KHEChar B = Codec->decode( Byte );
    drawByte( P, Byte, B, HTC );

    P->translate( -x, 0 );
  }
}


void KBufferColumn::paintMarking( QPainter *P, KSection Positions, int Index, int Flag )
{
  const QColorGroup &CG = View->colorGroup();

  paintRange( P, CG.text(), Positions, Flag );

  const QColor &BC = CG.base();
  // paint all the bytes affected
  for( int p=Positions.start(); p<=Positions.end(); ++p,++Index )
  {
    KPixelX x = relXOfPos( p );

    // draw the byte
    P->translate( x, 0 );
    char Byte = Buffer->datum( Index );
    KHEChar B = Codec->decode( Byte );
    drawByte( P, Byte, B, BC );

    P->translate( -x, 0 );
  }
}


// TODO: smarter calculation
void KBufferColumn::paintGrid( QPainter *P, KSection Range )
{
  int st = 0; // counter for spacing triggering
  P->setPen( Qt::black );
  // paint all the bytes affected
  for( int p=Range.start(); p<=Range.end(); ++p,++st )
  {
    KPixelX x = relXOfPos( p );

    // draw the byte
    P->translate( x, 0 );

    // spacing behind byte and vertical grid enabled?
    if( st == SpacingTrigger && p != LastPos )
      P->drawLine( VerticalGridX, 0, VerticalGridX, LineHeight-1 ) ;

    P->translate( -x, 0 );
  }
}


void KBufferColumn::paintRange( QPainter *P, const QColor &Color, KSection Positions, int Flag )
{
  KPixelX RangeX = Flag & StartsBefore ? relRightXOfPos( Positions.start()-1 ) + 1 : relXOfPos( Positions.start() );
  KPixelX RangeW = (Flag & EndsLater ? relXOfPos( Positions.end()+1 ): relRightXOfPos( Positions.end() ) + 1)  - RangeX;

  P->fillRect( RangeX,0,RangeW,LineHeight, QBrush(Color,Qt::SolidPattern) );
}


void KBufferColumn::paintByte( QPainter *P, int Index )
{
  char Byte = ( Index > -1 ) ? Buffer->datum( Index ) : EmptyByte;
  KHEChar B = Codec->decode( Byte );

  const QColorGroup &CG = View->colorGroup();
  QColor Color = CG.text();
  QBrush Brush( CG.base(), Qt::SolidPattern );

  if( Index > -1 )
  {
    if( Ranges->markingIncludes(Index) )
    {
      Brush.setColor( CG.text() );
      Color = CG.base();
    }
    else if( Ranges->selectionIncludes(Index) )
    {
      Brush.setColor( CG.highlight() );
      Color = CG.highlightedText();
    }
    else
    {
      Brush.setColor( CG.base() );
      Color = colorForChar( B );
    }
  }

  P->fillRect( 0,0,ByteWidth,LineHeight, Brush );

  if( Index > -1 )
    drawByte( P, Byte, B, Color );
}


void KBufferColumn::paintFramedByte( QPainter *P, int Index, KFrameStyle FrameStyle )
{
  paintByte( P, Index );
  char Byte = ( Index > -1 ) ? Buffer->datum( Index ) : EmptyByte;
  KHEChar B = Codec->decode( Byte );

  P->setPen( colorForChar(B) );
  if( FrameStyle == Frame )
    P->drawRect( 0, 0, ByteWidth, LineHeight );
  else if( FrameStyle == Left )
    P->drawLine( 0, 0, 0, LineHeight-1 );
  else
    P->drawLine( ByteWidth-1,0,ByteWidth-1,LineHeight-1 );
}


void KBufferColumn::paintCursor( QPainter *P, int Index )
{
  char Byte = ( Index > -1 ) ? Buffer->datum( Index ) : EmptyByte;
  KHEChar B = Codec->decode( Byte );
  P->fillRect( 0, 0, ByteWidth, LineHeight, QBrush(colorForChar(B),Qt::SolidPattern) );
}


void KBufferColumn::drawByte( QPainter *P, char /*Byte*/, KHEChar B, const QColor &Color ) const
{
  P->setPen( Color );
  P->drawText( 0, DigitBaseLine, B );
}


bool KBufferColumn::isSelected( KSection Range, KSection *Selection, unsigned int *Flag ) const
{
  KSection S;
  unsigned int F = 0;
  const KSection *OS = Ranges->firstOverlappingSelection( Range );
  if( !OS )
    return false;
  S = *OS;

  // does selection start before asked range?
  if( Range.start() > S.start() )
  {
    S.setStart( Range.start() );
    F |= StartsBefore;
  }

  // does selection go on behind asked range?
  if( Range.end() < S.end() )
  {
    S.setEnd( Range.end() );
    F |= EndsLater;
  }

  *Selection = S;
  *Flag = F;
  return true;
}


bool KBufferColumn::isMarked( KSection Range, KSection *Marking, unsigned int *Flag ) const
{
  KSection M;
  unsigned int F = 0;
  const KSection *OM = Ranges->overlappingMarking( Range );
  if( !OM )
    return false;
  M = *OM;

  // does selection start before asked range?
  if( Range.start() > M.start() )
  {
    M.setStart( Range.start() );
    F |= StartsBefore;
  }

  // does selection go on behind asked range?
  if( Range.end() < M.end() )
  {
    M.setEnd( Range.end() );
    F |= EndsLater;
  }

  *Marking = M;
  *Flag = F;
  return true;
}
