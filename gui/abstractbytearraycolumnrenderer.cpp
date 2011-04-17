/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractbytearraycolumnrenderer.h"

// lib
#include "oktetagui.h"
#include "bytearraytableranges.h"
#include "bytearraytablecursor.h"
#include "bytearraytablelayout.h"
#include "helper.h"
// lib
#include <abstractcolumnstylist.h>
// Okteta core
#include <bookmarkable.h>
#include <bookmarksconstiterator.h>
#include <bookmark.h>
#include <charcodec.h>
// KDE
#include <KColorScheme>
// Qt
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>


namespace Okteta
{

AbstractByteArrayColumnRenderer::AbstractByteArrayColumnRenderer( AbstractColumnStylist* stylist,
    AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges )
 : AbstractColumnRenderer( stylist ),
   mByteArrayModel( byteArrayModel ),
   mLayout( layout ),
   mRanges( ranges ),
   mBookmarks( qobject_cast<Bookmarkable*>(byteArrayModel) ),
   mDigitWidth( 0 ),
   mDigitBaseLine( 0 ),
   mFontMetrics( QFont() ),
   mByteWidth( 0 ),
   mByteSpacingWidth( DefaultByteSpacingWidth ),
   mGroupSpacingWidth( DefaultGroupSpacingWidth ),
   mNoOfGroupedBytes( DefaultNoOfGroupedBytes ),
   mLinePosLeftPixelX( 0 ),
   mLinePosRightPixelX( 0 ),
   mLastLinePos( 0 ),
   mByteTypeColored( true )
{
}


void AbstractByteArrayColumnRenderer::set( AbstractByteArrayModel* byteArrayModel )
{
    mByteArrayModel = byteArrayModel;
    mBookmarks = qobject_cast<Bookmarkable*>(byteArrayModel );
}


void AbstractByteArrayColumnRenderer::resetXBuffer()
{
    delete [] mLinePosLeftPixelX;
    delete [] mLinePosRightPixelX;

    mLastLinePos = mLayout->noOfBytesPerLine()-1;
    mLinePosLeftPixelX =  new PixelX[mLastLinePos+1];
    mLinePosRightPixelX = new PixelX[mLastLinePos+1];

    if( mLinePosLeftPixelX )
        recalcX();
}


void AbstractByteArrayColumnRenderer::setFontMetrics( const QFontMetrics& fontMetrics )
{
    mFontMetrics = fontMetrics;

    mDigitBaseLine = fontMetrics.ascent();
    mDigitWidth = fontMetrics.maxWidth();

    // recalculate depend sizes
    recalcByteWidth();

    if( mLinePosLeftPixelX )
        recalcX();
}


bool AbstractByteArrayColumnRenderer::setSpacing( PixelX byteSpacingWidth, int NoGB, PixelX groupSpacingWidth )
{
    // no changes?
    if( mByteSpacingWidth == byteSpacingWidth && mNoOfGroupedBytes == NoGB && mGroupSpacingWidth == groupSpacingWidth )
        return false;

    mByteSpacingWidth = byteSpacingWidth;
    mNoOfGroupedBytes = NoGB;
    mGroupSpacingWidth = groupSpacingWidth;

    // recalculate depend sizes
    if( mLinePosLeftPixelX )
        recalcX();

    return true;
}


bool AbstractByteArrayColumnRenderer::setByteSpacingWidth( PixelX byteSpacingWidth )
{
    // no changes?
    if( mByteSpacingWidth == byteSpacingWidth )
        return false;

    mByteSpacingWidth = byteSpacingWidth;

    // recalculate depend sizes
    if( mLinePosLeftPixelX )
        recalcX();

    return true;
}


bool AbstractByteArrayColumnRenderer::setNoOfGroupedBytes( int NoGB )
{
    // no changes?
    if( mNoOfGroupedBytes == NoGB )
        return false;

    mNoOfGroupedBytes = NoGB;

    if( mLinePosLeftPixelX )
        recalcX();
    return true;
}


bool AbstractByteArrayColumnRenderer::setGroupSpacingWidth( PixelX groupSpacingWidth )
{
    // no changes?
    if( mGroupSpacingWidth == groupSpacingWidth )
        return false;

    mGroupSpacingWidth = groupSpacingWidth;

    // recalculate depend sizes
    if( mLinePosLeftPixelX )
        recalcX();

    return true;
}


void AbstractByteArrayColumnRenderer::recalcByteWidth()
{
    setByteWidth( mDigitWidth );
}


void AbstractByteArrayColumnRenderer::recalcX()
{
    mSpacingTrigger = noOfGroupedBytes() > 0 ? noOfGroupedBytes()-1 : mLastLinePos+1; // last ensures to never trigger the spacing

    PixelX newWidth = 0;
    Size groupedBytes = 0;
    PixelX* PX = mLinePosLeftPixelX;
    PixelX* PRX = mLinePosRightPixelX;
    LinePosition p = 0;
    for( ; p<=mLastLinePos; ++PX, ++PRX, ++p, ++groupedBytes )
    {
        *PX = newWidth;
        newWidth += mByteWidth;
        *PRX = newWidth-1;

        // is there a space behind the actual byte (if it is not the last)?
        if( groupedBytes == mSpacingTrigger )
        {
            newWidth += mGroupSpacingWidth;
            groupedBytes = -1;
        }
        else
            newWidth += mByteSpacingWidth;
    }
    setWidth( mLinePosRightPixelX[mLastLinePos]+1 );
}


// TODO: why are inlined functions not available as symbols when defined before their use
//TODO: works not precisly for the byte rects but includes spacing and left and right
/*inline*/ LinePosition AbstractByteArrayColumnRenderer::linePositionOfX( PixelX PX ) const
{
    if( !mLinePosLeftPixelX )
        return NoByteFound;

    // translate
    PX -= x();
    // search backwards for the first byte that is equalleft to x
    for( LinePosition p=mLastLinePos; p>=0; --p )
        if( mLinePosLeftPixelX[p] <= PX )
            return p;

    return 0; //NoByteFound;
}


LinePosition AbstractByteArrayColumnRenderer::magneticLinePositionOfX( PixelX PX ) const
{
    if( !mLinePosLeftPixelX )
        return NoByteFound;

    // translate
    PX -= x();
    // search backwards for the first byte that is equalleft to x
    for( LinePosition p = mLastLinePos; p>=0; --p )
        if( mLinePosLeftPixelX[p] <= PX )
        {
            // are we close to the right?
            if( mLinePosRightPixelX[p]-PX < mDigitWidth/2 ) // TODO: perhaps cache also the middle xpos's
                ++p;
            return p;
        }

    return 0; //NoByteFound;
}


LinePositionRange AbstractByteArrayColumnRenderer::linePositionsOfX( PixelX PX, PixelX PW ) const
{
    if( !mLinePosLeftPixelX )
        return LinePositionRange();

    // translate
    PX -= x();
    const PixelX PRX = PX + PW - 1;

    LinePositionRange positions;
    // search backwards for the first byte that is equalleft to x
    for( LinePosition p=mLastLinePos; p>=0; --p )
        if( mLinePosLeftPixelX[p] <= PRX )
        {
            positions.setEnd( p );
            for( ; p>=0; --p )
                if( mLinePosLeftPixelX[p] <= PX )
                {
                    positions.setStart( p );
                    break;
                }
            break;
        }

    return positions;
}


PixelX AbstractByteArrayColumnRenderer::xOfLinePosition( LinePosition linePosition ) const
{
    return x() + ( mLinePosLeftPixelX ? mLinePosLeftPixelX[linePosition] : 0 );
}

PixelX AbstractByteArrayColumnRenderer::rightXOfLinePosition( LinePosition linePosition ) const
{
    return x() + ( mLinePosRightPixelX ? mLinePosRightPixelX[linePosition] : 0 );
}


LinePosition AbstractByteArrayColumnRenderer::linePositionOfColumnX( PixelX PX ) const
{
    if( !mLinePosLeftPixelX )
        return NoByteFound;

    // search backwards for the first byte that is equalleft to x
    for( LinePosition p=mLastLinePos; p>=0; --p )
        if( mLinePosLeftPixelX[p] <= PX )
            return p;

    return 0; //NoByteFound;
}


LinePositionRange AbstractByteArrayColumnRenderer::linePositionsOfColumnXs( PixelX pixelX, PixelX pixelWidth ) const
{
    if( ! mLinePosLeftPixelX )
        return LinePositionRange();

    const PixelX rightPixelX = pixelX + pixelWidth - 1;

    LinePositionRange positions;
    // search backwards for the first byte that is equalleft to x
    for( LinePosition p=mLastLinePos; p>=0; --p )
        if( mLinePosLeftPixelX[p] <= rightPixelX )
        {
            const LinePosition endPos = p;
            positions.setEnd( p );
            for( p=0; p<=endPos; ++p )
                if( mLinePosRightPixelX[p] >= pixelX )
                {
                    positions.setStart( p );
                    break;
                }
            break;
        }

    return positions;
}


PixelX AbstractByteArrayColumnRenderer::columnXOfLinePosition( LinePosition linePosition ) const
{
    return mLinePosLeftPixelX ? mLinePosLeftPixelX[linePosition] : 0;
}

PixelX AbstractByteArrayColumnRenderer::columnRightXOfLinePosition( LinePosition linePosition ) const
{
    return mLinePosRightPixelX ? mLinePosRightPixelX[linePosition] : 0;
}


PixelXRange AbstractByteArrayColumnRenderer::xsOfLinePositionsInclSpaces( const LinePositionRange& linePositions ) const
{
    const PixelX x = (linePositions.start()>0) ? rightXOfLinePosition( linePositions.nextBeforeStart() ) + 1 :
                                                 xOfLinePosition( linePositions.start() );
    const PixelX rightX = (linePositions.end()<mLastLinePos) ? xOfLinePosition( linePositions.nextBehindEnd() ) - 1 :
                                                               rightXOfLinePosition( linePositions.end() );
    return PixelXRange( x, rightX );
}


PixelXRange AbstractByteArrayColumnRenderer::columnXsOfLinePositionsInclSpaces( const LinePositionRange& linePositions ) const
{
    const PixelX x = (linePositions.start()>0) ? columnRightXOfLinePosition( linePositions.nextBeforeStart() ) + 1 :
                                                 columnXOfLinePosition( linePositions.start() );
    const PixelX rightX = (linePositions.end()<mLastLinePos) ? columnXOfLinePosition( linePositions.nextBehindEnd() ) - 1 :
                                                               columnRightXOfLinePosition( linePositions.end() );
    return PixelXRange( x, rightX  );
}


QRect AbstractByteArrayColumnRenderer::byteRect( const Coord& coord ) const
{
    const int x = xOfLinePosition( coord.pos() );
    const int y = lineHeight() * coord.line();
    const int w = mByteWidth;
    const int h = lineHeight();
    const QPoint point( x, y );
    const QSize size( w, h );

    const QRect result( point, size );
    return result;
}


void AbstractByteArrayColumnRenderer::prepareRendering( const PixelXRange& _Xs )
{
    PixelXRange Xs( _Xs );
    restrictToXSpan( &Xs );
    // translate
    Xs.moveBy( -x() );

    // store the values
    mRenderX = Xs.start();
    mRenderWidth = Xs.width();

    // get line linePositions to paint
    mRenderLinePositions = linePositionsOfColumnXs( mRenderX, mRenderWidth );
}


void AbstractByteArrayColumnRenderer::renderFirstLine( QPainter* painter, const PixelXRange& Xs, Line firstLineIndex )
{
    prepareRendering( Xs );

    mRenderLine = firstLineIndex;

    renderLinePositions( painter, mRenderLine++, mRenderLinePositions );
}


void AbstractByteArrayColumnRenderer::renderNextLine( QPainter* painter )
{
    renderLinePositions( painter, mRenderLine++, mRenderLinePositions );
}


void AbstractByteArrayColumnRenderer::renderLinePositions( QPainter* painter, Line lineIndex, const LinePositionRange& _linePositions )
{
    // clear background
    const unsigned int blankFlag =
        (_linePositions.start()!=0?StartsBefore:0) | (_linePositions.end()!=mLastLinePos?EndsLater:0);
    const QBrush& backgroundBrush = stylist()->palette().brush( QPalette::Base );

    renderRange( painter, backgroundBrush, _linePositions, blankFlag );

    // no bytes to paint?
    if( !mLayout->hasContent(lineIndex) )
        return;

    // Go through the lines TODO: handle first and last line more effeciently
    // check for leading and trailing spaces
    const LinePositionRange existingLinePositions = mLayout->linePositions( lineIndex );

    LinePositionRange linePositions = _linePositions;
    linePositions.restrictTo( existingLinePositions );
    const int firstLinePosition = linePositions.start();

    // check for leading and trailing spaces
    AddressRange byteIndizes =
        AddressRange::fromWidth( mLayout->indexAtCoord(Coord( linePositions.start(), lineIndex )), linePositions.width() );

    unsigned int selectionFlag = 0;
    unsigned int markingFlag = 0;
    AddressRange selectedRange;
    AddressRange markedRange;
    bool hasMarking = mRanges->hasMarking();
    bool hasSelection = mRanges->hasSelection();

//kDebug() << QString("painting linePositions (painter%1-%2L%3): ").arg(linePositions.start()).arg(linePositions.end()).arg(lineIndex)
//         <<linePositions.start()<<"-"<<linePositions.start()
//         <<" for byteIndizes "<<byteIndizes.start()<<"-"<<byteIndizes.start()<<endl;
    while( linePositions.isValid() )
    {
        LinePositionRange positionsPart = linePositions;  // set of linePositions to paint next
        AddressRange byteIndizesPart = byteIndizes;      // set of indizes to paint next

        if( hasMarking && markedRange.endsBefore(byteIndizesPart) )
            hasMarking = getNextMarkedAddressRange( &markedRange, &markingFlag, byteIndizesPart );

        if( hasSelection && selectedRange.endsBefore(byteIndizesPart) )
            hasSelection = getNextSelectedAddressRange( &selectedRange, &selectionFlag, byteIndizesPart );

        if( byteIndizesPart.start() == markedRange.start() )
        {
            byteIndizesPart.setEnd( markedRange.end() );
            positionsPart.setEndByWidth( markedRange.width() );

            if( positionsPart.start() == existingLinePositions.start() ) markingFlag &= ~StartsBefore;
            // TODO: hack: needed because otherwise the spacing will be plain
            else if( positionsPart.start() == firstLinePosition && selectedRange.includes(byteIndizesPart.start()) )
                renderSelectionSpaceBehind( painter, firstLinePosition-1 );

            if( positionsPart.end() == existingLinePositions.end() )     markingFlag &= ~EndsLater;
            // TODO: hack: needed because otherwise the spacing will be plain
            else if( positionsPart.end() == linePositions.end() && selectedRange.includes(byteIndizesPart.end()) )
                renderSelectionSpaceBehind( painter, positionsPart.end() );

            renderMarking( painter, positionsPart, byteIndizesPart.start(), markingFlag );
        }
        else if( selectedRange.includes(byteIndizesPart.start()) )
        {
            if( selectedRange.startsBefore(byteIndizesPart) )
                selectionFlag |= StartsBefore;

            const bool hasMarkingBeforeSelectionEnd = ( hasMarking && markedRange.start() <= selectedRange.end() );

            byteIndizesPart.setEnd( hasMarkingBeforeSelectionEnd ? markedRange.nextBeforeStart() : selectedRange.end() );
            positionsPart.setEndByWidth( byteIndizesPart.width() );

            if( hasMarkingBeforeSelectionEnd )
                selectionFlag |= EndsLater;
            if( positionsPart.start() == existingLinePositions.start() ) selectionFlag &= ~StartsBefore;
            if( positionsPart.end() == existingLinePositions.end() )     selectionFlag &= ~EndsLater;

            renderSelection( painter, positionsPart, byteIndizesPart.start(), selectionFlag );
        }
        else
        {
            // calc end of plain text
            if( hasMarking )
                byteIndizesPart.setEnd( markedRange.nextBeforeStart() );
            if( hasSelection )
                byteIndizesPart.restrictEndTo( selectedRange.nextBeforeStart() );

            positionsPart.setEndByWidth( byteIndizesPart.width() );

            renderPlain( painter, positionsPart, byteIndizesPart.start() );
        }

        byteIndizes.setStartNextBehind( byteIndizesPart );
        linePositions.setStartNextBehind( positionsPart );
    }
}


void AbstractByteArrayColumnRenderer::renderPlain( QPainter* painter, const LinePositionRange& linePositions, Address byteIndex )
{
    BookmarksConstIterator bit;
    Address nextBookmarkOffset = -1;

    const bool hasBookmarks = ( mBookmarks != 0 );
    if( hasBookmarks )
    {
        bit = mBookmarks->createBookmarksConstIterator();
        if( bit.findNextFrom(byteIndex) )
            nextBookmarkOffset = bit.next().offset();
    }

    const QPalette& palette = stylist()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::View );

    // paint all the bytes affected
    for( LinePosition linePosition=linePositions.start(); linePosition<=linePositions.end(); ++linePosition,++byteIndex )
    {
        const PixelX x = columnXOfLinePosition( linePosition );

        // draw the byte
        painter->translate( x, 0 );

        if( byteIndex == nextBookmarkOffset )
        {
            renderBookmark( painter, colorScheme.background(KColorScheme::NeutralBackground) );

            nextBookmarkOffset = bit.hasNext() ? bit.next().offset() : -1;//TODO )&& ( bit->offset() <= LastIndex );
        }

        const Byte byte = mByteArrayModel->byte( byteIndex );
        const Character byteChar = mCharCodec->decode( byte );

        const KColorScheme::ForegroundRole foregroundRole =
            mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
        const QBrush brush = colorScheme.foreground( foregroundRole );
        const QColor& charColor = brush.color();// palette.text().color();//colorForChar(byteChar)
        renderByteText( painter, byte, byteChar, charColor );

        painter->translate( -x, 0 );
    }
}


void AbstractByteArrayColumnRenderer::renderSelection( QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag )
{
    BookmarksConstIterator bit;
    Address nextBookmarkOffset = -1;

    const bool hasBookmarks = ( mBookmarks != 0 );
    if( hasBookmarks )
    {
        bit = mBookmarks->createBookmarksConstIterator();
        if( bit.findNextFrom( byteIndex) )
            nextBookmarkOffset = bit.next().offset();
    }

    const QPalette& palette = stylist()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::Selection );

    renderRange( painter, colorScheme.background(), linePositions, flag );

    // paint all the bytes affected
    for( LinePosition linePosition=linePositions.start(); linePosition<=linePositions.end(); ++linePosition,++byteIndex )
    {
        const PixelX x = columnXOfLinePosition( linePosition );

        // draw the byte
        painter->translate( x, 0 );

        if( byteIndex == nextBookmarkOffset )
        {
            renderBookmark( painter, colorScheme.background(KColorScheme::NeutralBackground) );

            nextBookmarkOffset = bit.hasNext() ? bit.next().offset() : -1;//TODO )&& ( bit->offset() <= LastIndex );
        }

        const Byte byte = mByteArrayModel->byte( byteIndex );
        const Character byteChar = mCharCodec->decode( byte );

        const KColorScheme::ForegroundRole foregroundRole =
            mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
        const QBrush brush = colorScheme.foreground( foregroundRole );
        const QColor& charColor = brush.color();
        renderByteText( painter, byte, byteChar, charColor );

        painter->translate( -x, 0 );
    }
}

void AbstractByteArrayColumnRenderer::renderSelectionSpaceBehind( QPainter* painter, LinePosition linePosition )
{
    const QPalette& palette = stylist()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::Selection );

    renderSpaceBehind( painter, colorScheme.background(), linePosition );
}


void AbstractByteArrayColumnRenderer::renderMarking( QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag )
{
    const QPalette& palette = stylist()->palette();

    renderRange( painter, palette.text(), linePositions, flag );

    const QColor& baseColor = palette.base().color();
    // paint all the bytes affected
    for( LinePosition p=linePositions.start(); p<=linePositions.end(); ++p,++byteIndex )
    {
        const PixelX x = columnXOfLinePosition( p );

        // draw the byte
        painter->translate( x, 0 );
        const Byte byte = mByteArrayModel->byte( byteIndex );
        const Character byteChar = mCharCodec->decode( byte );
        renderByteText( painter, byte, byteChar, baseColor );

        painter->translate( -x, 0 );
    }
}


void AbstractByteArrayColumnRenderer::renderBookmark( QPainter* painter, const QBrush& brush )
{
    // TODO: think about how bookmarks should really be rendered
    painter->fillRect( 1,1, mByteWidth-2,lineHeight()-2, brush );
}


void AbstractByteArrayColumnRenderer::renderRange( QPainter* painter, const QBrush& brush, const LinePositionRange& linePositions, int flag )
{
    const PixelX rangeX =
        ( flag & StartsBefore ) ? columnRightXOfLinePosition( linePositions.nextBeforeStart() ) + 1 :
                                  columnXOfLinePosition( linePositions.start() );
    const PixelX rangeW =
        ( (flag & EndsLater) ? columnXOfLinePosition( linePositions.nextBehindEnd() ) :
                               columnRightXOfLinePosition( linePositions.end() ) + 1  )
        - rangeX;

    painter->fillRect( rangeX,0, rangeW,lineHeight(), brush );
}

void AbstractByteArrayColumnRenderer::renderSpaceBehind( QPainter* painter, const QBrush& brush, LinePosition linePosition )
{
    const PixelX rangeX = columnRightXOfLinePosition( linePosition ) + 1;
    const PixelX rangeW = columnXOfLinePosition( linePosition+1 ) - rangeX;

    painter->fillRect( rangeX,0, rangeW,lineHeight(), brush );
}


void AbstractByteArrayColumnRenderer::renderByte( QPainter* painter, Address byteIndex )
{
    const Byte byte = ( byteIndex > -1 ) ? mByteArrayModel->byte( byteIndex ) : EmptyByte;
    const Character byteChar = mCharCodec->decode( byte );

    const QPalette& palette = stylist()->palette();

    KColorScheme::ColorSet colorSet = KColorScheme::View;
    if( byteIndex > -1 )
    {
        if( mRanges->selectionIncludes(byteIndex) )
            colorSet = KColorScheme::Selection;
//    else if( mRanges->markingIncludes(byteIndex) )
//    {
//      charColor = palette.base().color();
//      brush = palette.text();
//    }
    }
    KColorScheme colorScheme( palette.currentColorGroup(), colorSet );

    const QBrush backgroundBrush = colorScheme.background();
    painter->fillRect( 0,0, mByteWidth,lineHeight(), backgroundBrush );

    if( mBookmarks && mBookmarks->containsBookmarkFor(byteIndex) )
        renderBookmark( painter, colorScheme.background(KColorScheme::NeutralBackground) );

    if( byteIndex > -1 )
    {
        const KColorScheme::ForegroundRole foregroundRole =
            mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
        const QBrush brush = colorScheme.foreground( foregroundRole );
        const QColor& charColor = brush.color();

        renderByteText( painter, byte, byteChar, charColor );
    }
}

// TODO: think about making framestyle a enum of a class ByteArrayColumnCursor
void AbstractByteArrayColumnRenderer::renderFramedByte( QPainter* painter, Address byteIndex, FrameStyle frameStyle )
{
    renderByte( painter, byteIndex );

    const Byte byte = ( byteIndex > -1 ) ? mByteArrayModel->byte( byteIndex ) : EmptyByte;
    const Character byteChar = mCharCodec->decode( byte );

    const bool isInSelection = ( byteIndex > -1 && mRanges->selectionIncludes(byteIndex) );
    const KColorScheme::ColorSet colorSet = isInSelection ? KColorScheme::Selection : KColorScheme::View;

    const QPalette& palette = stylist()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), colorSet );
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground( foregroundRole );
    const QColor& charColor = brush.color();
    painter->setPen( charColor );
    if( frameStyle == Frame )
        painter->drawRect( 0,0, mByteWidth-1,lineHeight()-1 );
    else if( frameStyle == Left )
        painter->drawLine( 0,0, 0,lineHeight()-1 );
    else
        painter->drawLine( mByteWidth-1,0, mByteWidth-1,lineHeight()-1 );
}


void AbstractByteArrayColumnRenderer::renderCursor( QPainter* painter, Address byteIndex )
{
    const Byte byte = ( byteIndex > -1 ) ? mByteArrayModel->byte( byteIndex ) : EmptyByte;
    const Character byteChar = mCharCodec->decode( byte );

    const bool isInSelection = ( byteIndex > -1 && mRanges->selectionIncludes(byteIndex) );
    const KColorScheme::ColorSet colorSet = isInSelection ? KColorScheme::Selection : KColorScheme::View;

    const QPalette& palette = stylist()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), colorSet );
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground( foregroundRole );
    painter->fillRect( 0,0, mByteWidth,lineHeight(), brush );
}


bool AbstractByteArrayColumnRenderer::getNextSelectedAddressRange( AddressRange* _selection, unsigned int* _flag,
                                                                   const AddressRange& range ) const
{
    const AddressRange* overlappingSelectedSection = mRanges->firstOverlappingSelection( range );
    if( !overlappingSelectedSection )
        return false;

    AddressRange selectedRange = *overlappingSelectedSection;
    unsigned int flag = 0;

    // does selectedRange start before asked range?
    if( selectedRange.startsBefore(range) )
    {
        selectedRange.setStart( range.start() );
        flag |= StartsBefore;
    }

    // does selectedRange go on behind asked range?
    if( selectedRange.endsBehind(range) )
    {
        selectedRange.setEnd( range.end() );
        flag |= EndsLater;
    }

    *_selection = selectedRange;
    *_flag = flag;
    return true;
}


bool AbstractByteArrayColumnRenderer::getNextMarkedAddressRange( AddressRange* _markedSection, unsigned int* _flag,
                                                                 const AddressRange& range ) const
{
    const AddressRange* overlappingMarkedSection = mRanges->overlappingMarking( range );
    if( !overlappingMarkedSection )
        return false;

    unsigned int flag = 0;
    AddressRange markedRange = *overlappingMarkedSection;

    if( markedRange.startsBefore(range) )
    {
        markedRange.setStart( range.start() );
        flag |= StartsBefore;
    }

    if( markedRange.endsBehind(range) )
    {
        markedRange.setEnd( range.end() );
        flag |= EndsLater;
    }

    *_markedSection = markedRange;
    *_flag = flag;
    return true;
}


AbstractByteArrayColumnRenderer::~AbstractByteArrayColumnRenderer()
{
    delete [] mLinePosLeftPixelX;
    delete [] mLinePosRightPixelX;
}

}
