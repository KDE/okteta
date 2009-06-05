/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayrowcolumnrenderer.h"

// lib
#include "bytearraytableranges.h"
#include "bytearraytablecursor.h"
#include "bytearraytablelayout.h"
#include "helper.h"
// ColumnsView
#include <columnsview.h>
// Okteta core
#include <kbookmarkable.h>
#include <kbookmarksconstiterator.h>
#include <bookmark.h>
#include <charcodec.h>
#include <valuecodec.h>
// KDE
#include <KColorScheme>
// Qt
#include <QtGui/QPainter>


namespace KHEUI
{

static const unsigned int StartsBefore = 1;
static const unsigned int EndsLater = 2;
static const char EmptyByte = ' ';

static const int DefaultVisibleCodings = AbstractByteArrayView::ValueCodingId | AbstractByteArrayView::CharCodingId;

static const KPixelX DefaultCursorWidth = 2;
static const KPixelX DefaultByteSpacingWidth = 3;
static const KPixelX DefaultGroupSpacingWidth = 9;
static const KPixelX DefaultRowSpacingHeight = 0;
static const int DefaultNoOfGroupedBytes = 4;

static const int DefaultBinaryGapWidth = 1;

static const bool      DefaultShowingNonprinting = false;
static const QChar     DefaultSubstituteChar =  (char)'.';
static const QChar     DefaultUndefinedChar =   (char)'?';

ByteArrayRowColumnRenderer::ByteArrayRowColumnRenderer( ColumnsView* columnsView,
    KHECore::AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges )
 : ColumnRenderer( columnsView ),
   mByteArrayModel( byteArrayModel ),
   mLayout( layout ),
   mRanges( ranges ),
   mBookmarks( qobject_cast<KHECore::Bookmarkable*>(byteArrayModel) ),
   mVisibleCodings( DefaultVisibleCodings ),
   mDigitWidth( 0 ),
   mDigitBaseLine( 0 ),
   mDigitHeight( 0 ),
   mByteWidth( 0 ),
   mByteSpacingWidth( DefaultByteSpacingWidth ),
   mGroupSpacingWidth( DefaultGroupSpacingWidth ),
   mNoOfGroupedBytes( DefaultNoOfGroupedBytes ),
   mLinePosLeftPixelX( 0 ),
   mLinePosRightPixelX( 0 ),
   mLastLinePos( 0 ),
   mByteTypeColored( true ),
   mValueCodec( 0 ),
   mBinaryGapWidth( DefaultBinaryGapWidth ),
   mShowingNonprinting( DefaultShowingNonprinting ),
   mSubstituteChar( DefaultSubstituteChar ),
   mUndefinedChar( DefaultUndefinedChar )
{
}

AbstractByteArrayView::CodingTypeId ByteArrayRowColumnRenderer::codingIdofY( KPixelY y ) const
{
    return
        ( mVisibleCodings != AbstractByteArrayView::BothCodingsId ) ?
            (AbstractByteArrayView::CodingTypeId)mVisibleCodings :
        ( y < mDigitHeight ) ?
            AbstractByteArrayView::ValueCodingId :
            AbstractByteArrayView::CharCodingId;
}

KPixelY ByteArrayRowColumnRenderer::rowHeight() const
{
    const int noOfCodings = ( mVisibleCodings > 2 ) ? 2 : 1;
    return noOfCodings * mDigitHeight + DefaultRowSpacingHeight;
}

KPixelY ByteArrayRowColumnRenderer::yOfCodingId( AbstractByteArrayView::CodingTypeId codingId ) const
{
    int result = ( mVisibleCodings <= 2 ) ?                             0 :
                 ( codingId == AbstractByteArrayView::ValueCodingId ) ? 0 :
                                                                        mDigitHeight;

    return result;
}


void ByteArrayRowColumnRenderer::set( KHECore::AbstractByteArrayModel* byteArrayModel )
{
    mByteArrayModel = byteArrayModel;
    mBookmarks = qobject_cast<KHECore::Bookmarkable*>(byteArrayModel );
}


void ByteArrayRowColumnRenderer::resetXBuffer()
{
    delete [] mLinePosLeftPixelX;
    delete [] mLinePosRightPixelX;

    mLastLinePos = mLayout->noOfBytesPerLine()-1;
    mLinePosLeftPixelX =  new KPixelX[mLastLinePos+1];
    mLinePosRightPixelX = new KPixelX[mLastLinePos+1];

    if( mLinePosLeftPixelX )
        recalcX();
}

void ByteArrayRowColumnRenderer::setVisibleCodings( int visibleCodings )
{
    mVisibleCodings = visibleCodings;
}


void ByteArrayRowColumnRenderer::setMetrics( KPixelX digitWidth, KPixelY digitBaseLine, KPixelY digitHeight )
{
    mDigitBaseLine = digitBaseLine;
    mDigitHeight = digitHeight;
    setDigitWidth( digitWidth );
}


bool ByteArrayRowColumnRenderer::setDigitWidth( KPixelX digitWidth )
{
    // no changes?
    if( mDigitWidth == digitWidth )
        return false;

    mDigitWidth = digitWidth;
    // recalculate depend sizes
    recalcByteWidth();

    if( mLinePosLeftPixelX )
        recalcX();
    return true;
}


bool ByteArrayRowColumnRenderer::setSpacing( KPixelX byteSpacingWidth, int NoGB, KPixelX groupSpacingWidth )
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


bool ByteArrayRowColumnRenderer::setByteSpacingWidth( KPixelX byteSpacingWidth )
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


bool ByteArrayRowColumnRenderer::setNoOfGroupedBytes( int NoGB )
{
    // no changes?
    if( mNoOfGroupedBytes == NoGB )
        return false;

    mNoOfGroupedBytes = NoGB;

    if( mLinePosLeftPixelX )
        recalcX();
    return true;
}


bool ByteArrayRowColumnRenderer::setGroupSpacingWidth( KPixelX groupSpacingWidth )
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


void ByteArrayRowColumnRenderer::setValueCodec( KHECore::ValueCoding valueCoding, const KHECore::ValueCodec* valueCodec )
{
    mValueCoding = valueCoding;
    mValueCodec = valueCodec;
    mDecodedByteText.resize( mValueCodec->encodingWidth() );

    // recalculate depend sizes
    recalcByteWidth();

    if( mLinePosLeftPixelX )
        recalcX();
}


bool ByteArrayRowColumnRenderer::setBinaryGapWidth( KPixelX binaryGapWidth )
{
    // no changes?
    if( mBinaryGapWidth == binaryGapWidth )
        return false;

    mBinaryGapWidth = binaryGapWidth;

    // recalculate depend sizes
    recalcByteWidth();

    if( mLinePosLeftPixelX )
        recalcX();
    return true;
}


void ByteArrayRowColumnRenderer::recalcByteWidth()
{
    int byteWidth = mValueCodec->encodingWidth() * mDigitWidth;

    if( mValueCoding == KHECore::BinaryCoding )
    {
        mBinaryHalfOffset = 4 * mDigitWidth + mBinaryGapWidth;
        byteWidth += mBinaryGapWidth;
    }
    setByteWidth( byteWidth );
}

// perhaps sometimes there will be a grammar
void ByteArrayRowColumnRenderer::renderEditedByte( QPainter* painter, char byte, const QString& editBuffer )
{
    const KHECore::KChar byteChar = mCharCodec->decode( byte );

    const QPalette& palette = columnsView()->viewport()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::View );
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground( foregroundRole );
    painter->fillRect( 0,0, byteWidth(),mDigitHeight, brush );

    const QBrush backgroundBrush = colorScheme.background();
    const QColor& charColor = backgroundBrush.color();
    renderCode( painter, editBuffer, charColor );
}


void ByteArrayRowColumnRenderer::renderByteText( QPainter* painter,
                                                 char byte, KHECore::KChar byteChar, int codings,
                                                 const QColor& color ) const
{
    int charBaseLine = mDigitBaseLine;
    if( codings & AbstractByteArrayView::ValueCodingId )
    {
        mValueCodec->encode( mDecodedByteText, 0, byte );
        renderCode( painter, mDecodedByteText, color );

        charBaseLine += mDigitHeight;
    }

    if( codings & AbstractByteArrayView::CharCodingId )
    {
    // turn into a drawable String
        const QString text( byteChar.isUndefined() ?                      KHECore::KChar(mUndefinedChar) :
                           !(mShowingNonprinting || byteChar.isPrint()) ? KHECore::KChar(mSubstituteChar) :
                                                                          byteChar );

        painter->setPen( color );
        painter->drawText( 0, charBaseLine, text );
    }
}


void ByteArrayRowColumnRenderer::renderCode( QPainter* painter, const QString& code, const QColor& color ) const
{
    painter->setPen( color );
    if( mValueCoding == KHECore::BinaryCoding )
    {
        // leave a gap in the middle
        painter->drawText( 0, mDigitBaseLine, code.left(4) );
        painter->drawText( mBinaryHalfOffset, mDigitBaseLine, code.right(4) );
    }
    else
        painter->drawText( 0, mDigitBaseLine, code );
}


// char variant
// void ByteArrayRowColumnRenderer::recalcByteWidth()
// {
//     setByteWidth( mDigitWidth );
// }


void ByteArrayRowColumnRenderer::recalcX()
{
    mSpacingTrigger = noOfGroupedBytes() > 0 ? noOfGroupedBytes()-1 : mLastLinePos+1; // last ensures to never trigger the spacing

    KPixelX newWidth = 0;
    int groupedBytes = 0;
    KPixelX* PX = mLinePosLeftPixelX;
    KPixelX* PRX = mLinePosRightPixelX;
    int p = 0;
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
/*inline*/ int ByteArrayRowColumnRenderer::linePositionOfX( KPixelX PX ) const
{
    if( !mLinePosLeftPixelX )
        return NoByteFound;

    // translate
    PX -= x();
    // search backwards for the first byte that is equalleft to x
    for( int p=mLastLinePos; p>=0; --p )
        if( mLinePosLeftPixelX[p] <= PX )
            return p;

    return 0; //NoByteFound;
}


int ByteArrayRowColumnRenderer::magneticLinePositionOfX( KPixelX PX ) const
{
    if( !mLinePosLeftPixelX )
        return NoByteFound;

    // translate
    PX -= x();
    // search backwards for the first byte that is equalleft to x
    for( int p=mLastLinePos; p>=0; --p )
        if( mLinePosLeftPixelX[p] <= PX )
        {
            // are we close to the right?
            if( mLinePosRightPixelX[p]-PX < mDigitWidth/2 ) // TODO: perhaps cache also the middle xpos's
                ++p;
            return p;
        }

    return 0; //NoByteFound;
}


KHE::Section ByteArrayRowColumnRenderer::linePositionsOfX( KPixelX PX, KPixelX PW ) const
{
    if( !mLinePosLeftPixelX )
        return KHE::Section();

    // translate
    PX -= x();
    const int PRX = PX + PW - 1;

    KHE::Section positions;
    // search backwards for the first byte that is equalleft to x
    for( int p=mLastLinePos; p>=0; --p )
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

KPixelX ByteArrayRowColumnRenderer::xOfLinePosition( int linePosition ) const
{ return x() + (mLinePosLeftPixelX?mLinePosLeftPixelX[linePosition]:0); }

KPixelX ByteArrayRowColumnRenderer::rightXOfLinePosition( int linePosition ) const
{ return x() + (mLinePosRightPixelX?mLinePosRightPixelX[linePosition]:0); }


int ByteArrayRowColumnRenderer::linePositionOfColumnX( KPixelX PX ) const
{
    if( !mLinePosLeftPixelX )
        return NoByteFound;

    // search backwards for the first byte that is equalleft to x
    for( int p=mLastLinePos; p>=0; --p )
        if( mLinePosLeftPixelX[p] <= PX )
            return p;

    return 0; //NoByteFound;
}


KHE::Section ByteArrayRowColumnRenderer::linePositionsOfColumnXs( KPixelX PX, KPixelX PW ) const
{
    if( !mLinePosLeftPixelX )
        return KHE::Section();

    const int PRX = PX + PW - 1;

    KHE::Section positions;
    // search backwards for the first byte that is equalleft to x
    for( int p=mLastLinePos; p>=0; --p )
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


KPixelX ByteArrayRowColumnRenderer::columnXOfLinePosition( int linePosition ) const
{ return mLinePosLeftPixelX ? mLinePosLeftPixelX[linePosition] : 0; }
KPixelX ByteArrayRowColumnRenderer::columnRightXOfLinePosition( int linePosition ) const
{ return mLinePosRightPixelX ? mLinePosRightPixelX[linePosition] : 0; }


KPixelXs ByteArrayRowColumnRenderer::xsOfLinePositionsInclSpaces( const KHE::Section& linePositions ) const
{
    const int x = (linePositions.start()>0) ? rightXOfLinePosition( linePositions.nextBeforeStart() ) + 1 :
                                              xOfLinePosition( linePositions.start() );
    const int rightX = (linePositions.end()<mLastLinePos) ? xOfLinePosition( linePositions.nextBehindEnd() ) - 1 :
                                                            rightXOfLinePosition( linePositions.end() );
    return KPixelXs( x, rightX );
}


KPixelXs ByteArrayRowColumnRenderer::columnXsOfLinePositionsInclSpaces( const KHE::Section& linePositions ) const
{
    const int x = (linePositions.start()>0) ? columnRightXOfLinePosition( linePositions.nextBeforeStart() ) + 1 :
                                              columnXOfLinePosition( linePositions.start() );
    const int rightX = (linePositions.end()<mLastLinePos) ? columnXOfLinePosition( linePositions.nextBehindEnd() ) - 1 :
                                                            columnRightXOfLinePosition( linePositions.end() );
    return KPixelXs( x, rightX  );
}


void ByteArrayRowColumnRenderer::prepareRendering( const KPixelXs& _Xs )
{
    KPixelXs Xs( _Xs );
    restrictToXSpan( &Xs );
    // translate
    Xs.moveBy( -x() );

    // store the values
    mRenderX = Xs.start();
    mRenderWidth = Xs.width();

    // get line linePositions to paint
    mRenderLinePositions = linePositionsOfColumnXs( mRenderX, mRenderWidth );
}


void ByteArrayRowColumnRenderer::renderFirstLine( QPainter* painter, const KPixelXs& Xs, int firstLineIndex )
{
    prepareRendering( Xs );

    mRenderLine = firstLineIndex;

    renderLinePositions( painter, mRenderLine++, mRenderLinePositions );
}


void ByteArrayRowColumnRenderer::renderNextLine( QPainter* painter )
{
    renderLinePositions( painter, mRenderLine++, mRenderLinePositions );
}


void ByteArrayRowColumnRenderer::renderLinePositions( QPainter* painter, int lineIndex, const KHE::Section& _linePositions )
{
    // clear background
    const unsigned int blankFlag =
        (_linePositions.start()!=0?StartsBefore:0) | (_linePositions.end()!=mLastLinePos?EndsLater:0);
    const QWidget* viewport = columnsView()->viewport();
    const QBrush& backgroundBrush = viewport->palette().brush( viewport->backgroundRole() );

    renderRange( painter, backgroundBrush, _linePositions, blankFlag );

    // Go through the lines TODO: handle first and last line more effeciently
    // check for leading and trailing spaces
    KHE::Section linePositions( mLayout->firstLinePosition(Coord( _linePositions.start(), lineIndex )),
                                 mLayout->lastLinePosition( Coord( _linePositions.end(),   lineIndex )) );

    // no bytes to paint?
    if( !mLayout->hasContent(lineIndex) )
        return;

    // check for leading and trailing spaces
    KHE::Section byteIndizes =
        KHE::Section::fromWidth( mLayout->indexAtCoord(Coord( linePositions.start(), lineIndex )), linePositions.width() );

    unsigned int selectionFlag = 0;
    unsigned int markingFlag = 0;
    KHE::Section selection;
    KHE::Section markedSection;
    bool hasMarking = mRanges->hasMarking();
    bool hasSelection = mRanges->hasSelection();

//kDebug() << QString("painting linePositions (painter%1-%2L%3): ").arg(linePositions.start()).arg(linePositions.end()).arg(lineIndex)
//         <<linePositions.start()<<"-"<<linePositions.start()
//         <<" for byteIndizes "<<byteIndizes.start()<<"-"<<byteIndizes.start()<<endl;
    while( linePositions.isValid() )
    {
        KHE::Section positionsPart( linePositions );  // set of linePositions to paint next
        KHE::Section byteIndizesPart( byteIndizes );      // set of indizes to paint next
        // falls markedSection nicht mehr gebuffert und noch zu erwarten
        if( hasMarking && markedSection.endsBefore(byteIndizesPart.start()) )
        {
            // erhebe nächste Markierung im Bereich
            hasMarking = isMarked( byteIndizesPart, &markedSection, &markingFlag );
        }
        // falls selection nicht mehr gebuffert und noch zu erwarten
        if( hasSelection && selection.endsBefore(byteIndizesPart.start()) )
        {
            // erhebe nächste selection im Bereich
            hasSelection = isSelected( byteIndizesPart, &selection, &selectionFlag );
        }

        if( markedSection.start() == byteIndizesPart.start() )
        {
            byteIndizesPart.setEnd( markedSection.end() );
            positionsPart.setEndByWidth( markedSection.width() );
            if( positionsPart.end() == mLayout->lastLinePosition(lineIndex) )   markingFlag &= ~EndsLater;
            if( positionsPart.start() == mLayout->firstLinePosition(lineIndex)) markingFlag &= ~StartsBefore;
            renderMarking( painter, positionsPart, byteIndizesPart.start(), markingFlag );
        }
        else if( selection.includes(byteIndizesPart.start()) )
        {
            if( selection.startsBehind(byteIndizesPart.start()) )
                selectionFlag |= StartsBefore;
            bool MarkingBeforeEnd = hasMarking && markedSection.start() <= selection.end();

            byteIndizesPart.setEnd( MarkingBeforeEnd ? markedSection.nextBeforeStart() : selection.end() );
            positionsPart.setEndByWidth( byteIndizesPart.width() );

            if( MarkingBeforeEnd )
                selectionFlag |= EndsLater;
            if( positionsPart.end() == mLayout->lastLinePosition(lineIndex) )    selectionFlag &= ~EndsLater;
            if( positionsPart.start() == mLayout->firstLinePosition(lineIndex) ) selectionFlag &= ~StartsBefore;

            renderSelection( painter, positionsPart, byteIndizesPart.start(), selectionFlag );
        }
        else
        {
            // calc end of plain text
            if( hasMarking )
                byteIndizesPart.setEnd( markedSection.nextBeforeStart() );
            if( hasSelection )
                byteIndizesPart.restrictEndTo( selection.nextBeforeStart() );

            positionsPart.setEndByWidth( byteIndizesPart.width() );
            renderPlain( painter, positionsPart, byteIndizesPart.start() );
        }
        byteIndizes.setStartNextBehind( byteIndizesPart );
        linePositions.setStartNextBehind( positionsPart );
    }
}


void ByteArrayRowColumnRenderer::renderPlain( QPainter* painter, const KHE::Section& linePositions, int byteIndex )
{
    KHECore::BookmarksConstIterator bit;
    int nextBookmarkOffset = -1;

    const bool hasBookmarks = ( mBookmarks != 0 );
    if( hasBookmarks )
    {
        bit = mBookmarks->createBookmarksConstIterator();
        if( bit.findNextFrom(byteIndex) )
            nextBookmarkOffset = bit.next().offset();
    }

    const QPalette& palette = columnsView()->viewport()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::View );

    // paint all the bytes affected
    for( int linePosition=linePositions.start(); linePosition<=linePositions.end(); ++linePosition,++byteIndex )
    {
        const KPixelX x = columnXOfLinePosition( linePosition );

        // draw the byte
        painter->translate( x, 0 );

        if( byteIndex == nextBookmarkOffset )
        {
            renderBookmark( painter, colorScheme.background(KColorScheme::NeutralBackground) );

            nextBookmarkOffset = bit.hasNext() ? bit.next().offset() : -1;//TODO )&& ( bit->offset() <= LastIndex );
        }

        const char byte = mByteArrayModel->datum( byteIndex );
        const KHECore::KChar byteChar = mCharCodec->decode( byte );

        const KColorScheme::ForegroundRole foregroundRole =
            mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
        const QBrush brush = colorScheme.foreground( foregroundRole );
        const QColor& charColor = brush.color();// palette.text().color();//colorForChar(byteChar)
        renderByteText( painter, byte, byteChar, mVisibleCodings, charColor );

        painter->translate( -x, 0 );
    }
}


void ByteArrayRowColumnRenderer::renderSelection( QPainter* painter, const KHE::Section& linePositions, int byteIndex, int flag )
{
    KHECore::BookmarksConstIterator bit;
    int nextBookmarkOffset = -1;

    const bool hasBookmarks = ( mBookmarks != 0 );
    if( hasBookmarks )
    {
        bit = mBookmarks->createBookmarksConstIterator();
        if( bit.findNextFrom( byteIndex) )
            nextBookmarkOffset = bit.next().offset();
    }

    const QPalette& palette = columnsView()->viewport()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::Selection );

    renderRange( painter, colorScheme.background(), linePositions, flag );

    // paint all the bytes affected
    for( int linePosition=linePositions.start(); linePosition<=linePositions.end(); ++linePosition,++byteIndex )
    {
        const KPixelX x = columnXOfLinePosition( linePosition );

        // draw the byte
        painter->translate( x, 0 );

        if( byteIndex == nextBookmarkOffset )
        {
            renderBookmark( painter, colorScheme.background(KColorScheme::NeutralBackground) );

            nextBookmarkOffset = bit.hasNext() ? bit.next().offset() : -1;//TODO )&& ( bit->offset() <= LastIndex );
        }

        const char byte = mByteArrayModel->datum( byteIndex );
        const KHECore::KChar byteChar = mCharCodec->decode( byte );

        const KColorScheme::ForegroundRole foregroundRole =
            mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
        const QBrush brush = colorScheme.foreground( foregroundRole );
        const QColor& charColor = brush.color();
        renderByteText( painter, byte, byteChar, mVisibleCodings, charColor );

        painter->translate( -x, 0 );
    }
}


void ByteArrayRowColumnRenderer::renderMarking( QPainter* painter, const KHE::Section& linePositions, int byteIndex, int flag )
{
    const QPalette& palette = columnsView()->viewport()->palette();

    renderRange( painter, palette.text(), linePositions, flag );

    const QColor& baseColor = palette.base().color();
    // paint all the bytes affected
    for( int p=linePositions.start(); p<=linePositions.end(); ++p,++byteIndex )
    {
        const KPixelX x = columnXOfLinePosition( p );

        // draw the byte
        painter->translate( x, 0 );
        const char byte = mByteArrayModel->datum( byteIndex );
        const KHECore::KChar byteChar = mCharCodec->decode( byte );
        renderByteText( painter, byte, byteChar, mVisibleCodings, baseColor );

        painter->translate( -x, 0 );
    }
}


void ByteArrayRowColumnRenderer::renderBookmark( QPainter* painter, const QBrush& brush )
{
    // TODO: think about how bookmarks should really be rendered
    painter->fillRect( 1,1, mByteWidth-2,lineHeight()-2, brush );
}


void ByteArrayRowColumnRenderer::renderRange( QPainter* painter, const QBrush& brush, const KHE::Section& linePositions, int flag )
{
    const KPixelX rangeX =
        ( flag & StartsBefore ) ? columnRightXOfLinePosition( linePositions.nextBeforeStart() ) + 1 :
                                  columnXOfLinePosition( linePositions.start() );
    const KPixelX rangeW =
        ( (flag & EndsLater) ? columnXOfLinePosition( linePositions.nextBehindEnd() ) :
                               columnRightXOfLinePosition( linePositions.end() ) + 1  )
        - rangeX;

    painter->fillRect( rangeX,0, rangeW,lineHeight(), brush );
}


void ByteArrayRowColumnRenderer::renderByte( QPainter* painter,
                                             int byteIndex, AbstractByteArrayView::CodingTypeId codingId )
{
    const char byte = ( byteIndex > -1 ) ? mByteArrayModel->datum( byteIndex ) : EmptyByte;
    const KHECore::KChar byteChar = mCharCodec->decode( byte );

    const QWidget* viewport = columnsView()->viewport();
    const QPalette& palette = viewport->palette();

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
    painter->fillRect( 0,0, mByteWidth,mDigitHeight, backgroundBrush );

    if( mBookmarks && mBookmarks->containsBookmarkFor(byteIndex) )
    {
        const QBrush bookmarkBackgroundBrush = colorScheme.background( KColorScheme::NeutralBackground );
        painter->fillRect( 1,1, mByteWidth-2,mDigitHeight-2, bookmarkBackgroundBrush );
    }

    if( byteIndex > -1 )
    {
        const KColorScheme::ForegroundRole foregroundRole =
            mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
        const QBrush brush = colorScheme.foreground( foregroundRole );
        const QColor& charColor = brush.color();

        renderByteText( painter, byte, byteChar, codingId, charColor );
    }
}

// TODO: think about making framestyle a enum of a class ByteArrayColumnCursor
void ByteArrayRowColumnRenderer::renderFramedByte( QPainter* painter,
                                                   int byteIndex, AbstractByteArrayView::CodingTypeId codingId,
                                                   FrameStyle frameStyle )
{
    renderByte( painter, byteIndex, codingId );

    const char byte = ( byteIndex > -1 ) ? mByteArrayModel->datum( byteIndex ) : EmptyByte;
    const KHECore::KChar byteChar = mCharCodec->decode( byte );

    const bool isInSelection = ( byteIndex > -1 && mRanges->selectionIncludes(byteIndex) );
    const KColorScheme::ColorSet colorSet = isInSelection ? KColorScheme::Selection : KColorScheme::View;

    const QPalette& palette = columnsView()->viewport()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), colorSet );
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground( foregroundRole );
    const QColor& charColor = brush.color();
    painter->setPen( charColor );
    if( frameStyle == Frame )
        painter->drawRect( 0,0, mByteWidth-1,mDigitHeight-1 );
    else if( frameStyle == Left )
        painter->drawLine( 0,0, 0,mDigitHeight-1 );
    else
        painter->drawLine( mByteWidth-1,0, mByteWidth-1,mDigitHeight-1 );
}


void ByteArrayRowColumnRenderer::renderCursor( QPainter* painter, int byteIndex, AbstractByteArrayView::CodingTypeId codingId )
{
Q_UNUSED( codingId )
    const char byte = ( byteIndex > -1 ) ? mByteArrayModel->datum( byteIndex ) : EmptyByte;
    const KHECore::KChar byteChar = mCharCodec->decode( byte );

    const bool isInSelection = ( byteIndex > -1 && mRanges->selectionIncludes(byteIndex) );
    const KColorScheme::ColorSet colorSet = isInSelection ? KColorScheme::Selection : KColorScheme::View;

    const QPalette& palette = columnsView()->viewport()->palette();
    KColorScheme colorScheme( palette.currentColorGroup(), colorSet );
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar): KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground( foregroundRole );
    painter->fillRect( 0,0, mByteWidth,mDigitHeight, brush );
}


bool ByteArrayRowColumnRenderer::isSelected( const KHE::Section& section, KHE::Section* _selection,
                                                  unsigned int* _flag ) const
{
    const KHE::Section* overlappingSelectedSection = mRanges->firstOverlappingSelection( section );
    if( !overlappingSelectedSection )
        return false;

    KHE::Section selection = *overlappingSelectedSection;
    unsigned int flag = 0;

    // does selection start before asked range?
    if( selection.startsBefore(section) )
    {
        selection.setStart( section.start() );
        flag |= StartsBefore;
    }

    // does selection go on behind asked range?
    if( selection.endsBehind(section) )
    {
        selection.setEnd( section.end() );
        flag |= EndsLater;
    }

    *_selection = selection;
    *_flag = flag;
    return true;
}


bool ByteArrayRowColumnRenderer::isMarked( const KHE::Section& section, KHE::Section* _markedSection,
                                                unsigned int* _flag ) const
{
    const KHE::Section* overlappingMarkedSection = mRanges->overlappingMarking( section );
    if( !overlappingMarkedSection )
        return false;

    unsigned int flag = 0;
    KHE::Section markedSection = *overlappingMarkedSection;

    if( markedSection.startsBefore(section) )
    {
        markedSection.setStart( section.start() );
        flag |= StartsBefore;
    }

    if( markedSection.endsBehind(section) )
    {
        markedSection.setEnd( section.end() );
        flag |= EndsLater;
    }

    *_markedSection = markedSection;
    *_flag = flag;
    return true;
}


ByteArrayRowColumnRenderer::~ByteArrayRowColumnRenderer()
{
    delete [] mLinePosLeftPixelX;
    delete [] mLinePosRightPixelX;
}

}
