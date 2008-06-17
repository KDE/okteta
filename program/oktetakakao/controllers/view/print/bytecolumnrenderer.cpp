/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2003,2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytecolumnrenderer.h"

// lib
#include "helper.h"
// Okteta gui
#include <bytearraytablelayout.h>
// Okteta core
// #include <kibookmarks.h>
#include <kcharcodec.h>
// Qt
#include <QtGui/QPainter>


namespace KHEPrint
{

static const unsigned int StartsBefore = 1;
static const unsigned int EndsLater = 2;
static const char EmptyByte = ' ';

static const KPixelX DefaultCursorWidth = 2;
static const KPixelX DefaultByteSpacingWidth = 3;
static const KPixelX DefaultGroupSpacingWidth = 9;
static const int DefaultNoOfGroupedBytes = 4;

ByteColumnRenderer::ByteColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                                        const KHECore::KAbstractByteArrayModel *byteArray,
                                        const KHE::KSection &renderIndizes,
                                        const KHEUI::ByteArrayTableLayout *layout )
 : AbstractColumnRenderer( columnFrameRenderer ),
   mByteArrayModel( byteArray ),
   mRenderIndizes( renderIndizes ),
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


void ByteColumnRenderer::setByteArrayModel( const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                            const KHE::KSection &renderIndizes )
{
    mByteArrayModel = byteArrayModel;
    mRenderIndizes = renderIndizes;
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
/*inline*/ int ByteColumnRenderer::linePositionOfX( KPixelX PX ) const
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


int ByteColumnRenderer::magneticLinePositionOfX( KPixelX PX ) const
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


KHE::KSection ByteColumnRenderer::linePositionsOfX( KPixelX PX, KPixelX PW ) const
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


KPixelX ByteColumnRenderer::xOfLinePosition( int linePositions )      const { return x() + (mPosX?mPosX[linePositions]:0); }
KPixelX ByteColumnRenderer::rightXOfLinePosition( int linePositions ) const { return x() + (mPosRightX?mPosRightX[linePositions]:0); }


int ByteColumnRenderer::linePositionOfColumnX( KPixelX PX ) const
{
    if( !mPosX )
       return NoByteFound;

    // search backwards for the first byte that is equalleft to x
    for( int p=mLastPos; p>=0; --p )
        if( mPosX[p] <= PX )
            return p;

    return 0; //NoByteFound;
}


KHE::KSection ByteColumnRenderer::linePositionsOfColumnXs( KPixelX PX, KPixelX PW ) const
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


KPixelX ByteColumnRenderer::columnXOfLinePosition( int linePositions )      const { return mPosX ? mPosX[linePositions] : 0; }
KPixelX ByteColumnRenderer::columnRightXOfLinePosition( int linePositions ) const { return mPosRightX ? mPosRightX[linePositions] : 0; }


KPixelXs ByteColumnRenderer::xsOfLinePositionsInclSpaces( const KHE::KSection &positions ) const
{
    const int firstX = positions.startsBehind( 0 ) ? rightXOfLinePosition( positions.nextBeforeStart() ) + 1 :
                                                     xOfLinePosition( positions.start() );
    const int lastX = positions.endsBefore( mLastPos ) ? xOfLinePosition( positions.nextBehindEnd() ) - 1 :
                                                         rightXOfLinePosition( positions.end() );
    return KPixelXs( firstX, lastX  );
}


KPixelXs ByteColumnRenderer::columnXsOfLinePositionsInclSpaces( const KHE::KSection &positions ) const
{
    const int firstX = positions.startsBehind( 0 ) ? columnRightXOfLinePosition( positions.nextBeforeStart() ) + 1 :
                                                     columnXOfLinePosition( positions.start() );
    const int lastX = positions.endsBefore( mLastPos ) ? columnXOfLinePosition( positions.nextBehindEnd() ) - 1 :
                                                         columnRightXOfLinePosition( positions.end() );
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
    mRenderPositions = linePositionsOfColumnXs( mRenderX, mRenderW );
}


void ByteColumnRenderer::renderFirstLine( QPainter *painter, const KPixelXs &Xs, int FirstLine )
{
    prepareRendering( Xs );

    mRenderLine = FirstLine;

    renderLinePositions( painter, mRenderLine++, mRenderPositions );
}


void ByteColumnRenderer::renderNextLine( QPainter *painter )
{
    renderLinePositions( painter, mRenderLine++, mRenderPositions );
}


void ByteColumnRenderer::renderLinePositions( QPainter *painter, int Line, const KHE::KSection &linePositions )
{
    // clear background
//     const unsigned int isBlank = (linePositions.start()!=0?StartsBefore:0) | (linePositions.end()!=mLastPos?EndsLater:0);

//     renderRange( painter, Qt::white, linePositions, isBlank );

    // Go through the lines TODO: handle first and last line more effeciently
    // check for leading and trailing spaces
    KHE::KSection positions( mLayout->firstLinePosition(Coord( linePositions.start(), Line )),
                             mLayout->lastLinePosition( Coord( linePositions.end(),  Line )) );

    // no bytes to paint?
    if( !mLayout->hasContent(Line) )
        return;

    // check for leading and trailing spaces
    KHE::KSection indices =
        KHE::KSection::fromWidth( mLayout->indexAtCoord(Coord( positions.start(), Line )), positions.width() );

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
    for( int linePositions=positions.start(); linePositions<=positions.end(); ++linePositions,++index )
    {
        KPixelX x = columnXOfLinePosition( linePositions );

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

        char byte = mByteArrayModel->datum( mRenderIndizes.start()+index );
        KHECore::KChar byteChar = mCodec->decode( byte );

        drawByte( painter, byte, byteChar, colorForChar(byteChar) );

        painter->translate( -x, 0 );
    }
}


void ByteColumnRenderer::renderRange( QPainter *painter, const QBrush &Brush, const KHE::KSection &positions, int Flag )
{
    KPixelX RangeX = Flag & StartsBefore ? columnRightXOfLinePosition( positions.nextBeforeStart() ) + 1 : columnXOfLinePosition( positions.start() );
    KPixelX RangeW = (Flag & EndsLater ? columnXOfLinePosition( positions.nextBehindEnd() ): columnRightXOfLinePosition( positions.end() ) + 1)  - RangeX;

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

}
