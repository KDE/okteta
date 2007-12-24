/***************************************************************************
                          bytecolumnrenderer.cpp  -  description
                             -------------------
    begin                : Mit Mai 14 2003
    copyright            : 2003,2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "bytecolumnrenderer.h"

// lib
#include "kdatalayout.h"
#include "helper.h"
// Okteta core
// #include <kibookmarks.h>
#include <kcharcodec.h>
// Qt
#include <QtGui/QPainter>


static const unsigned int StartsBefore = 1;
static const unsigned int EndsLater = 2;
static const char EmptyByte = ' ';

static const KPixelX DefaultCursorWidth = 2;
static const KPixelX DefaultByteSpacingWidth = 3;
static const KPixelX DefaultGroupSpacingWidth = 9;
static const int DefaultNoOfGroupedBytes = 4;

ByteColumnRenderer::ByteColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                                        const KHECore::KAbstractByteArrayModel *byteArray,
                                        const KHEUI::KDataLayout *layout )
 : AbstractColumnRenderer( columnFrameRenderer ),
   mByteArrayModel( byteArray ),
   mLayout( layout ),
//    Bookmarks( qobject_cast<KDE::If::Bookmarks*>(ByteArray) ),
   mDigitWidth( 0 ),
   mDigitBaseLine( 0 ),
   mByteWidth( 0 ),
   mByteSpacingWidth( DefaultByteSpacingWidth ),
   mGroupSpacingWidth( DefaultGroupSpacingWidth ),
   mNoOfGroupedBytes( DefaultNoOfGroupedBytes ),
   mPosX( 0 ),
   mPosRightX( 0 ),
   mLastPos( 0 )
{
}


void ByteColumnRenderer::setByteArrayModel( const KHECore::KAbstractByteArrayModel *byteArrayModel )
{
    mByteArrayModel = byteArrayModel;
//     Bookmarks = qobject_cast<KDE::If::Bookmarks*>(mByteArrayModel );
}


void ByteColumnRenderer::resetXBuffer()
{
    delete [] mPosX;
    delete [] mPosRightX;

    mLastPos = mLayout->noOfBytesPerLine()-1;
    mPosX =      new KPixelX[mLastPos+1];
    mPosRightX = new KPixelX[mLastPos+1];

    if( mPosX )
        recalcX();
}


void ByteColumnRenderer::setMetrics( KPixelX digitWidth, KPixelY digitBaseLine )
{
    mDigitBaseLine = digitBaseLine;
    setDigitWidth( digitWidth );
}


bool ByteColumnRenderer::setDigitWidth( KPixelX digitWidth )
{
    // no changes?
    if( mDigitWidth == digitWidth )
        return false;

    mDigitWidth = digitWidth;
    // recalculate depend sizes
    recalcByteWidth();

    if( mPosX )
        recalcX();
    return true;
}


bool ByteColumnRenderer::setSpacing( KPixelX byteSpacingWidth, int noOfGroupedBytes, KPixelX groupSpacingWidth )
{
    // no changes?
    if( mByteSpacingWidth == byteSpacingWidth
        && mNoOfGroupedBytes == noOfGroupedBytes
        && mGroupSpacingWidth == groupSpacingWidth )
        return false;

    mByteSpacingWidth = byteSpacingWidth;
    mNoOfGroupedBytes = noOfGroupedBytes;
    mGroupSpacingWidth = groupSpacingWidth;

    // recalculate depend sizes
    if( mPosX )
        recalcX();

    return true;
}


bool ByteColumnRenderer::setByteSpacingWidth( KPixelX byteSpacingWidth )
{
    // no changes?
    if( mByteSpacingWidth == byteSpacingWidth )
        return false;

    mByteSpacingWidth = byteSpacingWidth;

    // recalculate depend sizes
    if( mPosX )
        recalcX();

    return true;
}


bool ByteColumnRenderer::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    // no changes?
    if( mNoOfGroupedBytes == noOfGroupedBytes )
        return false;

    mNoOfGroupedBytes = noOfGroupedBytes;

    if( mPosX )
        recalcX();
    return true;
}


bool ByteColumnRenderer::setGroupSpacingWidth( KPixelX groupSpacingWidth )
{
    // no changes?
    if( mGroupSpacingWidth == groupSpacingWidth )
        return false;

    mGroupSpacingWidth = groupSpacingWidth;

    // recalculate depend sizes
    if( mPosX )
        recalcX();

    return true;
}


void ByteColumnRenderer::recalcByteWidth()
{
    mByteWidth = mDigitWidth;
}


void ByteColumnRenderer::recalcX()
{
    mSpacingTrigger = noOfGroupedBytes() > 0 ? noOfGroupedBytes()-1 : mLastPos+1; // last ensures to never trigger the spacing

    KPixelX newWidth = 0;
    int p = 0;
    int gs = 0;
    KPixelX *PX = mPosX;
    KPixelX *PRX = mPosRightX;
    for( ; PX<&mPosX[mLastPos+1]; ++PX, ++PRX, ++p, ++gs )
    {
        *PX = newWidth;
        newWidth += mByteWidth;
        *PRX = newWidth-1;

        // is there a space behind the actual byte (if it is not the last)?
        if( gs == mSpacingTrigger )
        {
            newWidth += mGroupSpacingWidth;
            gs = -1;
        }
        else
            newWidth += mByteSpacingWidth;
    }
    setWidth( mPosRightX[mLastPos]+1 );
}


// TODO: why are inlined functions not available as symbols when defined before their use
//TODO: works not precisly for the byte rects but includes spacing and left and right
/*inline*/ int ByteColumnRenderer::posOfX( KPixelX PX ) const
{
    if( !mPosX )
        return NoByteFound;

    // translate
    PX -= x();
    // search backwards for the first byte that is equalleft to x
    for( int p=mLastPos; p>=0; --p )
        if( mPosX[p] <= PX )
            return p;

    return 0; //NoByteFound;
}


int ByteColumnRenderer::magPosOfX( KPixelX PX ) const
{
    if( !mPosX )
        return NoByteFound;

    // translate
    PX -= x();
    // search backwards for the first byte that is equalleft to x
    for( int p=mLastPos; p>=0; --p )
        if( mPosX[p] <= PX )
        {
            // are we close to the right?
            if( mPosRightX[p]-PX < mDigitWidth/2 ) // TODO: perhaps cache also the middle xpos's
                ++p;
            return p;
        }

    return 0; //NoByteFound;
}


KHE::KSection ByteColumnRenderer::posOfX( KPixelX PX, KPixelX PW ) const
{
    if( !mPosX )
       return KHE::KSection();

    // translate
    PX -= x();
    const int PRX = PX + PW - 1;

    KHE::KSection P;
    // search backwards for the first byte that is equalleft to x
    for( int p=mLastPos; p>=0; --p )
        if( mPosX[p] <= PRX )
        {
            P.setEnd( p );
            for( ; p>=0; --p )
                if( mPosX[p] <= PX )
                {
                    P.setStart( p );
                    break;
                }
            break;
        }

  return P;
}


KPixelX ByteColumnRenderer::xOfPos( int Pos )      const { return x() + (mPosX?mPosX[Pos]:0); }
KPixelX ByteColumnRenderer::rightXOfPos( int Pos ) const { return x() + (mPosRightX?mPosRightX[Pos]:0); }


int ByteColumnRenderer::posOfRelX( KPixelX PX ) const
{
    if( !mPosX )
       return NoByteFound;

    // search backwards for the first byte that is equalleft to x
    for( int p=mLastPos; p>=0; --p )
        if( mPosX[p] <= PX )
            return p;

    return 0; //NoByteFound;
}


KHE::KSection ByteColumnRenderer::posOfRelX( KPixelX PX, KPixelX PW ) const
{
    if( !mPosX )
        return KHE::KSection();

    const int PRX = PX + PW - 1;

    KHE::KSection P;
    // search backwards for the first byte that is equalleft to x
    for( int p=mLastPos; p>=0; --p )
        if( mPosX[p] <= PRX )
        {
        P.setEnd( p );
        for( ; p>=0; --p )
            if( mPosX[p] <= PX )
            {
                P.setStart( p );
                break;
            }
        break;
        }

    return P;
}


KPixelX ByteColumnRenderer::relXOfPos( int Pos )      const { return mPosX ? mPosX[Pos] : 0; }
KPixelX ByteColumnRenderer::relRightXOfPos( int Pos ) const { return mPosRightX ? mPosRightX[Pos] : 0; }


KPixelXs ByteColumnRenderer::wideXPixelsOfPos( const KHE::KSection &positions ) const
{
    const int firstX = positions.startsBehind( 0 ) ? rightXOfPos( positions.beforeStart() ) + 1 :
                                                     xOfPos( positions.start() );
    const int lastX = positions.endsBefore( mLastPos ) ? xOfPos( positions.behindEnd() ) - 1 :
                                                         rightXOfPos( positions.end() );
    return KPixelXs( firstX, lastX  );
}


KPixelXs ByteColumnRenderer::relWideXPixelsOfPos( const KHE::KSection &positions ) const
{
    const int firstX = positions.startsBehind( 0 ) ? relRightXOfPos( positions.beforeStart() ) + 1 :
                                                     relXOfPos( positions.start() );
    const int lastX = positions.endsBefore( mLastPos ) ? relXOfPos( positions.behindEnd() ) - 1 :
                                                         relRightXOfPos( positions.end() );
    return KPixelXs( firstX, lastX  );
}


void ByteColumnRenderer::prepareRendering( const KPixelXs &_Xs )
{
    KPixelXs Xs( _Xs );
    restrictToXSpan( &Xs );
    // translate
    Xs.moveBy( -x() );

    // store the values
    mRenderX = Xs.start();
    mRenderW = Xs.width();

    // get line positions to paint
    mRenderPositions = posOfRelX( mRenderX, mRenderW );
}


void ByteColumnRenderer::renderFirstLine( QPainter *painter, const KPixelXs &Xs, int FirstLine )
{
    prepareRendering( Xs );

    mRenderLine = FirstLine;

    renderPositions( painter, mRenderLine++, mRenderPositions );
}


void ByteColumnRenderer::renderNextLine( QPainter *painter )
{
    renderPositions( painter, mRenderLine++, mRenderPositions );
}


void ByteColumnRenderer::renderPositions( QPainter *painter, int Line, const KHE::KSection &Pos )
{
    // clear background
//     const unsigned int isBlank = (Pos.start()!=0?StartsBefore:0) | (Pos.end()!=mLastPos?EndsLater:0);

//     renderRange( painter, Qt::white, Pos, isBlank );

    // Go through the lines TODO: handle first and last line more effeciently
    // check for leading and trailing spaces
    KHE::KSection positions( mLayout->firstPos(KCoord( Pos.start(), Line )),
                             mLayout->lastPos( KCoord( Pos.end(),  Line )) );

    // no bytes to paint?
    if( !mLayout->hasContent(Line) )
        return;

    // check for leading and trailing spaces
    KHE::KSection indices =
        KHE::KSection::fromWidth( mLayout->indexAtCoord(KCoord( positions.start(), Line )), positions.width() );

    positions.setEndByWidth( indices.width() );
    renderPlain( painter, positions, indices.start() );
}


void ByteColumnRenderer::renderPlain( QPainter *painter, const KHE::KSection &positions, int index )
{
#if 0
  bool hasBookmarks = ( Bookmarks != 0 );
  KHECore::KBookmarkList bookmarkList;
  KHECore::KBookmarkList::ConstIterator bit;
  if( hasBookmarks )
  {
    bookmarkList = Bookmarks->bookmarkList();
    bit = bookmarkList.nextFrom(index);
    hasBookmarks = ( bit != bookmarkList.constEnd() );
  }
#endif
    // paint all the bytes affected
    for( int Pos=positions.start(); Pos<=positions.end(); ++Pos,++index )
    {
        KPixelX x = relXOfPos( Pos );

        // draw the byte
        painter->translate( x, 0 );

#if 0
    if( hasBookmarks && (index == bit->offset()) )
    {
      paintBookmark( painter );
      ++bit;
      hasBookmarks = (bit != bookmarkList.constEnd());//TODO )&& ( bit->offset() <= LastIndex );
    }
#endif

        char byte = mByteArrayModel->datum( index );
        KHECore::KChar byteChar = mCodec->decode( byte );

        drawByte( painter, byte, byteChar, colorForChar(byteChar) );

        painter->translate( -x, 0 );
    }
}


void ByteColumnRenderer::renderRange( QPainter *painter, const QBrush &Brush, const KHE::KSection &positions, int Flag )
{
    KPixelX RangeX = Flag & StartsBefore ? relRightXOfPos( positions.start()-1 ) + 1 : relXOfPos( positions.start() );
    KPixelX RangeW = (Flag & EndsLater ? relXOfPos( positions.end()+1 ): relRightXOfPos( positions.end() ) + 1)  - RangeX;

    painter->fillRect( RangeX,0, RangeW,lineHeight(), Brush );
}


void ByteColumnRenderer::drawByte( QPainter *painter, char /*Byte*/, KHECore::KChar byteChar, const QColor &color ) const
{
    painter->setPen( color );
    painter->drawText( 0, mDigitBaseLine, byteChar );
}


ByteColumnRenderer::~ByteColumnRenderer()
{
    delete [] mPosX;
    delete [] mPosRightX;
}
