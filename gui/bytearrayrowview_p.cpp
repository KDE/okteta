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

#include "bytearrayrowview_p.h"

// lib
#include "bordercolumnrenderer.h"
#include "bytearraytableranges.h"
#include "bytearraytablelayout.h"
#include "controller/kvalueeditor.h"
#include "controller/dropper.h"
#include "kcursor.h"
// Okteta core
#include <abstractbytearraymodel.h>
#include <valuecodec.h>
#include <charcodec.h>
#include <wordbytearrayservice.h>
// KDE
#include <KGlobalSettings>
// Qt
#include <QtGui/QApplication>
#include <QtGui/QStyle>
#include <QtGui/QPainter>
#include <QtGui/QCursor>
#include <QtGui/QClipboard>
#include <QtGui/QScrollBar>
#include <QtGui/QMouseEvent>
#include <QtCore/QListIterator>
#include <QtCore/QTimer>


namespace KHEUI
{
static const int DefaultScrollTimerPeriod = 100;
static const int InsertCursorWidth = 2;


ByteArrayRowViewPrivate::ByteArrayRowViewPrivate( ByteArrayRowView* parent )
: AbstractByteArrayViewPrivate( parent ),
   mCursorPixmaps( new KCursor() ),
   mMousePressed( false ),
   mInDoubleClick( false ),
   mInDnD( false ),
   mDragStartPossible( false ),
   mBlinkCursorVisible( false )
{
}

void ByteArrayRowViewPrivate::init()
{
   Q_Q( ByteArrayRowView );

    mCursorBlinkTimer = new QTimer( q );
    mScrollTimer = new QTimer( q );
    mDragStartTimer = new QTimer( q );
    mTrippleClickTimer = new QTimer( q );

    // creating the columns in the needed order
    mOffsetColumn =
        new OffsetColumnRenderer( q, mTableLayout, KOffsetFormat::Hexadecimal );
    mBorderColumn =
        new BorderColumnRenderer( q, false );
    mByteArrayColumn =
        new ByteArrayRowColumnRenderer( q, mByteArrayModel, mTableLayout, mTableRanges );

    // select the active column
    mActiveCoding = AbstractByteArrayView::CharCodingId;
    mInactiveCoding = AbstractByteArrayView::ValueCodingId;

    // set char encoding
    mByteArrayColumn->setValueCodec( (KHECore::ValueCoding)mValueCoding, mValueCodec );
    mByteArrayColumn->setCharCodec( mCharCodec );
//     mByteArrayColumn->setActiveCoding( mActiveCoding );

    adaptController();

    q->setFont( KGlobalSettings::fixedFont() );

    q->connect( mCursorBlinkTimer, SIGNAL(timeout()), q, SLOT(blinkCursor()) );
    q->connect( mScrollTimer,      SIGNAL(timeout()), q, SLOT(autoScrollTimerDone()) );
    q->connect( mDragStartTimer,   SIGNAL(timeout()), q, SLOT(startDrag()) );
    mDragStartTimer->setSingleShot( true );
    mTrippleClickTimer->setSingleShot( true );

    q->setAcceptDrops( true );
}

int ByteArrayRowViewPrivate::visibleCodings()                          const { return mByteArrayColumn->visibleCodings(); }
ByteArrayRowView::CodingTypeId ByteArrayRowViewPrivate::activeCoding() const { return mActiveCoding; }

void ByteArrayRowViewPrivate::setByteArrayModel( KHECore::AbstractByteArrayModel* _byteArrayModel )
{
    mValueEditor->reset();

    // TODO: this fails if _byteArrayModel == null
    mByteArrayColumn->set( _byteArrayModel );

    AbstractByteArrayViewPrivate::setByteArrayModel( _byteArrayModel );
}


void ByteArrayRowViewPrivate::setBufferSpacing( KPixelX ByteSpacing, int noOfGroupedBytes, KPixelX GroupSpacing )
{
    if( !mByteArrayColumn->setSpacing(ByteSpacing,noOfGroupedBytes,GroupSpacing) )
        return;

    updateViewByWidth();
}


void ByteArrayRowViewPrivate::setValueCoding( AbstractByteArrayView::ValueCoding valueCoding )
{
    Q_Q( ByteArrayRowView );

    if( mValueCoding == valueCoding )
        return;

    const uint oldCodingWidth = mValueCodec->encodingWidth();

    AbstractByteArrayViewPrivate::setValueCoding( valueCoding );

    mByteArrayColumn->setValueCodec( (KHECore::ValueCoding)mValueCoding, mValueCodec );
    mValueEditor->adaptToValueCodecChange();

    const uint newCodingWidth = mValueCodec->encodingWidth();

    // no change in the width?
    if( newCodingWidth == oldCodingWidth )
        q->updateColumn( *mByteArrayColumn );
    else
        updateViewByWidth();

    emit q->valueCodingChanged( valueCoding );
}


void ByteArrayRowViewPrivate::setByteSpacingWidth( int/*KPixelX*/ byteSpacingWidth )
{
    if( !mByteArrayColumn->setByteSpacingWidth(byteSpacingWidth) )
        return;
    updateViewByWidth();
}

void ByteArrayRowViewPrivate::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    if( !mByteArrayColumn->setNoOfGroupedBytes(noOfGroupedBytes) )
        return;
    updateViewByWidth();
}


void ByteArrayRowViewPrivate::setGroupSpacingWidth( int/*KPixelX*/ groupSpacingWidth )
{
    if( !mByteArrayColumn->setGroupSpacingWidth(groupSpacingWidth) )
        return;
    updateViewByWidth();
}


void ByteArrayRowViewPrivate::setBinaryGapWidth( int/*KPixelX*/ binaryGapWidth )
{
    if( !mByteArrayColumn->setBinaryGapWidth(binaryGapWidth) )
        return;
    updateViewByWidth();
}


void ByteArrayRowViewPrivate::setSubstituteChar( QChar substituteChar )
{
    Q_Q( ByteArrayRowView );

    if( !mByteArrayColumn->setSubstituteChar(substituteChar) )
        return;
    pauseCursor();
    q->updateColumn( *mByteArrayColumn );
    unpauseCursor();
}

void ByteArrayRowViewPrivate::setUndefinedChar( QChar undefinedChar )
{
    Q_Q( ByteArrayRowView );

    if( !mByteArrayColumn->setUndefinedChar(undefinedChar) )
        return;
    pauseCursor();
    q->updateColumn( *mByteArrayColumn );
    unpauseCursor();
}

void ByteArrayRowViewPrivate::setShowsNonprinting( bool showingNonprinting )
{
    Q_Q( ByteArrayRowView );

    if( !mByteArrayColumn->setShowingNonprinting(showingNonprinting) )
        return;
    pauseCursor();
    q->updateColumn( *mByteArrayColumn );
    unpauseCursor();
}


void ByteArrayRowViewPrivate::setCharCoding( AbstractByteArrayView::CharCoding charCoding )
{
    Q_Q( ByteArrayRowView );

    if( mCharCoding == charCoding )
        return;

    AbstractByteArrayViewPrivate::setCharCoding( charCoding );

    pauseCursor();

    mByteArrayColumn->setCharCodec( mCharCodec );

    q->updateColumn( *mByteArrayColumn );

    unpauseCursor();

    emit q->charCodecChanged( charCodingName() );
}

// TODO: join with function above!
void ByteArrayRowViewPrivate::setCharCoding( const QString& newCharCodingName )
{
    Q_Q( ByteArrayRowView );

    if( charCodingName() == newCharCodingName )
        return;

    AbstractByteArrayViewPrivate::setCharCoding( newCharCodingName );

    pauseCursor();

    mByteArrayColumn->setCharCodec( mCharCodec );

    q->updateColumn( *mByteArrayColumn );

    unpauseCursor();

    emit q->charCodecChanged( charCodingName() );
}

void ByteArrayRowViewPrivate::setByteTypeColored( bool isColored )
{
    Q_Q( ByteArrayRowView );

    if( isColored == mByteArrayColumn->isByteTypeColored() )
        return;

    mByteArrayColumn->setByteTypeColored( isColored );

    pauseCursor();
    q->updateColumn( *mByteArrayColumn );
    unpauseCursor();
}


void ByteArrayRowViewPrivate::handleFontChange( const QFont& oldFont )
{
    Q_Q( ByteArrayRowView );

    q->AbstractByteArrayView::fontChange( oldFont );

    // get new values
    const QFontMetrics newFontMetrics = q->fontMetrics();
    const KPixelX digitWidth = newFontMetrics.maxWidth();
    const KPixelY digitBaseLine = newFontMetrics.ascent();
    const KPixelY digitHeight = newFontMetrics.height();

    mOffsetColumn->setMetrics( digitWidth, digitBaseLine );
    mByteArrayColumn->setMetrics( digitWidth, digitBaseLine, digitHeight );

    const int rowHeight = mByteArrayColumn->rowHeight();

    q->setLineHeight( rowHeight );

    // update all dependant structures
    mTableLayout->setNoOfLinesPerPage( q->noOfLinesPerPage() );

    updateViewByWidth();
}


void ByteArrayRowViewPrivate::adjustToLayoutNoOfBytesPerLine()
{
    Q_Q( ByteArrayRowView );

    mByteArrayColumn->resetXBuffer();

    q->updateWidths();
}


void ByteArrayRowViewPrivate::toggleOffsetColumn( bool showOffsetColumn )
{
    const bool isVisible = mOffsetColumn->isVisible();
    // no change?
    if( isVisible == showOffsetColumn )
        return;

    mOffsetColumn->setVisible( showOffsetColumn );

    updateViewByWidth();
}


QSize ByteArrayRowViewPrivate::minimumSizeHint() const
{
    Q_Q( const ByteArrayRowView );

    // TODO: better minimal width (visibility!)
    return QSize(
        mOffsetColumn->visibleWidth()
        + mBorderColumn->visibleWidth()
        + mByteArrayColumn->byteWidth(),
        q->lineHeight()
        + q->noOfLines()>1? q->style()->pixelMetric(QStyle::PM_ScrollBarExtent):0 );
}


int ByteArrayRowViewPrivate::fittingBytesPerLine() const
{
    Q_Q( const ByteArrayRowView );

    const QSize newSize = q->maximumViewportSize();
    const KPixelX reservedWidth =
        mOffsetColumn->visibleWidth()
        + mBorderColumn->visibleWidth();

    // abstract offset and border columns width
    const KPixelX fullWidth = newSize.width() - reservedWidth;

    //  // no width left for resizeable columns? TODO: put this in resizeEvent
    //  if( fullWidth < 0 )
    //    return;

    const KPixelY fullHeight = newSize.height();

    // check influence of dis-/appearing of the vertical scrollbar
    const bool verticalScrollbarIsVisible = q->verticalScrollBar()->isVisible();
    const KPixelX scrollbarExtent = q->style()->pixelMetric( QStyle::PM_ScrollBarExtent );

    KPixelX availableWidth = fullWidth;
    if( verticalScrollbarIsVisible )
        availableWidth -= scrollbarExtent;

    enum KMatchTrial { FirstRun, RerunWithScrollbarOn, TestWithoutScrollbar };
    KMatchTrial matchRun = FirstRun;

    // prepare needed values
    const KPixelX byteWidth = mByteArrayColumn->byteWidth();
    const KPixelX byteSpacingWidth = mByteArrayColumn->byteSpacingWidth();
    KPixelX groupSpacingWidth;
    int noOfGroupedBytes = mByteArrayColumn->noOfGroupedBytes();
    // no grouping?
    if( noOfGroupedBytes == 0 )
    {
        // faking grouping by 1
        noOfGroupedBytes = 1;
        groupSpacingWidth = 0;
    }
    else
        groupSpacingWidth = mByteArrayColumn->groupSpacingWidth();

    const KPixelX byteGroupWidth =  noOfGroupedBytes * byteWidth + (noOfGroupedBytes-1)*byteSpacingWidth;
    const KPixelX totalGroupWidth = byteGroupWidth + groupSpacingWidth;

    int fittingBytesPerLine;
    int fittingBytesPerLineWithScrollbar = 0;
    for(;;)
    {
    //    kDebug() << "matchWidth: " << fullWidth
    //              << " (v:" << visibleWidth()
    //              << ", f:" << frameWidth()
    //              << ", A:" << availableWidth
    //              << ", S:" << scrollbarExtent
    //              << ", R:" << reservedWidth << ")" << endl;

        // calculate fitting groups per line
        const int fittingGroupsPerLine = (availableWidth+groupSpacingWidth) // fake spacing after last group
                                          / totalGroupWidth;

        // calculate the fitting bytes per line by groups
        fittingBytesPerLine = noOfGroupedBytes * fittingGroupsPerLine;

        // not only full groups?
        if( mResizeStyle == AbstractByteArrayView::FullSizeUsage && noOfGroupedBytes > 1 )
        {

            if( fittingGroupsPerLine > 0 )
                availableWidth -= fittingGroupsPerLine*totalGroupWidth; // includes additional spacing after last group

//         kDebug() << "Left: " << availableWidth << "("<<byteWidth<<")" ;

            if( availableWidth > 0 )
                fittingBytesPerLine += (availableWidth+byteSpacingWidth) / (byteWidth+byteSpacingWidth);

            // is there not even the space for a single byte?
            if( fittingBytesPerLine == 0 )
            {
                // ensure at least one byte per line
                fittingBytesPerLine = 1;
                // and
                break;
            }
        }
        // is there not the space for a single group?
        else if( fittingBytesPerLine == 0 )
        {
            // ensures at least one group
            fittingBytesPerLine = noOfGroupedBytes;
            break;
        }

//    kDebug() << "meantime: " << fittingGroupsPerLine << " (T:" << totalGroupWidth
//              << ", h:" << byteGroupWidth
//              << ", s:" << groupSpacingWidth << ") " <<fittingBytesPerLine<< endl;

        const int newNoOfLines = (mTableLayout->length()+mTableLayout->startOffset()+fittingBytesPerLine-1)
                                 / fittingBytesPerLine;
        const KPixelY newHeight =  newNoOfLines * q->lineHeight();

        if( verticalScrollbarIsVisible )
        {
            if( matchRun == TestWithoutScrollbar )
            {
                // did the test without the scrollbar fail, don't the data fit into the view?
                if( newHeight>fullHeight )
                    // reset to old calculated value
                    fittingBytesPerLine =  fittingBytesPerLineWithScrollbar;
                break;
            }

            // a chance for to perhaps fit in height?
            if( fittingBytesPerLine <= mTableLayout->noOfBytesPerLine() )
            {
                // remember this trial's result and calc number of bytes with vertical scrollbar on
                fittingBytesPerLineWithScrollbar = fittingBytesPerLine;
                availableWidth = fullWidth;
                matchRun = TestWithoutScrollbar;
        //          kDebug() << "tested without scrollbar..." ;
                continue;
            }
        }
        else
        {
            // doesn't it fit into the height anymore?
            if( newHeight>fullHeight && matchRun==FirstRun )
            {
                // need for a scrollbar has risen... ->less width, new calculation
                availableWidth = fullWidth - scrollbarExtent;
                matchRun = RerunWithScrollbarOn;
        //          kDebug() << "rerun with scrollbar on..." ;
                continue;
            }
        }

        break;
    }

    return fittingBytesPerLine;
}

void ByteArrayRowViewPrivate::setVisibleCodings( int newCodings )
{
    Q_Q( ByteArrayRowView );

    const int oldCodings = visibleCodings();

    // no changes or no column selected?
    if( newCodings == oldCodings || !(newCodings&AbstractByteArrayView::BothCodingsId) )
        return;

    mByteArrayColumn->setVisibleCodings( newCodings );

    // active coding not visible anymore?
    const bool isActiveStillVisible = ( mActiveCoding & newCodings );
    if( !isActiveStillVisible )
    {
        mActiveCoding = (AbstractByteArrayView::CodingTypeId)newCodings;
        mInactiveCoding = AbstractByteArrayView::NoCodingId;
        adaptController();
    }
    else
        mInactiveCoding = (AbstractByteArrayView::CodingTypeId)( newCodings ^ mActiveCoding );

    const int rowHeight = mByteArrayColumn->rowHeight();
    q->setLineHeight( rowHeight );

    updateViewByWidth();
}


void ByteArrayRowViewPrivate::setActiveCoding( AbstractByteArrayView::CodingTypeId codingId )
{
    // no changes or not visible?
    if( codingId == mActiveCoding )
        return;

    pauseCursor();
    mValueEditor->finishEdit();

    mActiveCoding = codingId;
    mInactiveCoding = (AbstractByteArrayView::CodingTypeId)( visibleCodings() ^ codingId );

    adaptController();

    ensureCursorVisible();
    unpauseCursor();
}


void ByteArrayRowViewPrivate::placeCursor( const QPoint& point )
{
    Q_Q( ByteArrayRowView );

    const int lineIndex = q->lineAt( point.y() );
    const KPixelY lineY = lineIndex * q->lineHeight();
    const KPixelY y = point.y() - lineY;

    const AbstractByteArrayView::CodingTypeId codingId = mByteArrayColumn->codingIdofY( y );

    // switch active column if needed
    if( codingId != mActiveCoding )
    {
        mActiveCoding = codingId;
        mInactiveCoding = ( codingId != AbstractByteArrayView::ValueCodingId ) ? AbstractByteArrayView::ValueCodingId :
            AbstractByteArrayView::CharCodingId;
    }

    adaptController();

    // get coord of click and whether this click was closer to the end of the pos
    const int linePosition = mByteArrayColumn->magneticLinePositionOfX( point.x() );
    const Coord coord( linePosition, lineIndex );

    mTableCursor->gotoCCoord( coord );
    emit q->cursorPositionChanged( cursorPosition() );
}


int ByteArrayRowViewPrivate::indexByPoint( const QPoint& point ) const
{
    Q_Q( const ByteArrayRowView );

    const int lineIndex = q->lineAt( point.y() );
    const int linePosition = mByteArrayColumn->linePositionOfX( point.x() );

    const Coord coord( linePosition, lineIndex );

    return mTableLayout->indexAtCCoord( coord );
}


void ByteArrayRowViewPrivate::blinkCursor()
{
    // skip the cursor drawing?
    if( mCursorPaused || mValueEditor->isInEditMode() )
        return;

    // switch the cursor state
    mBlinkCursorVisible = !mBlinkCursorVisible;
    updateCursor( *mByteArrayColumn );
}


void ByteArrayRowViewPrivate::startCursor()
{
    mCursorPaused = false;

    updateCursors();

    mCursorBlinkTimer->start( QApplication::cursorFlashTime()/2 );
}


void ByteArrayRowViewPrivate::unpauseCursor()
{
    mCursorPaused = false;

    if( mCursorBlinkTimer->isActive() )
        updateCursors();
}


void ByteArrayRowViewPrivate::updateCursors()
{
    createCursorPixmaps();

    mBlinkCursorVisible = true;
    updateCursor( *mByteArrayColumn );
}


void ByteArrayRowViewPrivate::stopCursor()
{
    mCursorBlinkTimer->stop();

    pauseCursor();
}


void ByteArrayRowViewPrivate::pauseCursor()
{
    mCursorPaused = true;

    mBlinkCursorVisible = false;
    updateCursor( *mByteArrayColumn );
}


// TODO: should be movable to base class
void ByteArrayRowViewPrivate::updateCursor( const ByteArrayRowColumnRenderer& column )
{
    Q_Q( ByteArrayRowView );

    const int x = column.xOfLinePosition( mTableCursor->pos() ) - q->xOffset();
    const int y = q->lineHeight() * mTableCursor->line() - q->yOffset();
    const int w = column.byteWidth();

    q->viewport()->update( x,y, w,q->lineHeight() );
}

void ByteArrayRowViewPrivate::createCursorPixmaps()
{
    Q_Q( ByteArrayRowView );

    // create mCursorPixmaps
    mCursorPixmaps->setSize( mByteArrayColumn->byteWidth(), mByteArrayColumn->digitHeight() );

    const int index = mTableCursor->validIndex();

    QPainter painter;
    painter.begin( &mCursorPixmaps->offPixmap() );
    painter.initFrom( q );
    mByteArrayColumn->renderByte( &painter, index, mActiveCoding );
    painter.end();

    painter.begin( &mCursorPixmaps->onPixmap() );
    painter.initFrom( q );
    mByteArrayColumn->renderCursor( &painter, index, mActiveCoding );
    painter.end();

    // calculat the shape
    KPixelX cursorX;
    KPixelX cursorW;
    if( isCursorBehind() )
    {
        cursorX = qMax( 0, mCursorPixmaps->onPixmap().width()-InsertCursorWidth );
        cursorW = InsertCursorWidth;
    }
    else
    {
        cursorX = 0;
        cursorW = mOverWrite ? -1 : InsertCursorWidth;
    }
    mCursorPixmaps->setShape( cursorX, cursorW );
}


void ByteArrayRowViewPrivate::drawActiveCursor( QPainter* painter )
{
    Q_Q( ByteArrayRowView );

    // any reason to skip the cursor drawing?
    if( mBlinkCursorVisible && !q->hasFocus() && !q->viewport()->hasFocus() && !mDropper->isActive() )
        return;

    const int x = mByteArrayColumn->xOfLinePosition( mTableCursor->pos() );
    const int y = q->lineHeight() * mTableCursor->line()
                  + mByteArrayColumn->yOfCodingId( mActiveCoding );

    painter->translate( x, y );

    // paint edited byte?
    if( mValueEditor->isInEditMode() )
    {
        const int index = mTableCursor->index();

        if( mBlinkCursorVisible )
            mByteArrayColumn->renderEditedByte( painter, mValueEditor->value(), mValueEditor->valueAsString() );
        else
            mByteArrayColumn->renderByte( painter, index, mActiveCoding );
    }
    else
        painter->drawPixmap( mCursorPixmaps->cursorX(), 0,
                             mBlinkCursorVisible?mCursorPixmaps->onPixmap():mCursorPixmaps->offPixmap(),
                             mCursorPixmaps->cursorX(),0,mCursorPixmaps->cursorW(),-1 );

    painter->translate( -x, -y );
}


void ByteArrayRowViewPrivate::drawInactiveCursor( QPainter* painter )
{
    Q_Q( ByteArrayRowView );

    // any reason to skip the cursor drawing?
    if( mInactiveCoding == AbstractByteArrayView::NoCodingId
        || mCursorPaused
        || (!mCursorPaused && !q->hasFocus() && !q->viewport()->hasFocus() && !mDropper->isActive())  )
        return;

    const int index = mTableCursor->validIndex();

    const int x = mByteArrayColumn->xOfLinePosition( mTableCursor->pos() );
    const int y = q->lineHeight() * mTableCursor->line()
                  + mByteArrayColumn->yOfCodingId(mInactiveCoding);
    painter->translate( x, y );

    const ByteArrayRowColumnRenderer::FrameStyle frameStyle =
        mTableCursor->isBehind() ?                   ByteArrayRowColumnRenderer::Right :
        (mOverWrite||mValueEditor->isInEditMode()) ? ByteArrayRowColumnRenderer::Frame :
                                                     ByteArrayRowColumnRenderer::Left;
    mByteArrayColumn->renderFramedByte( painter, index, mInactiveCoding, frameStyle );

    painter->translate( -x, -y );
}


void ByteArrayRowViewPrivate::renderColumns( QPainter* painter, int cx, int cy, int cw, int ch )
{
    Q_Q( ByteArrayRowView );

    q->AbstractByteArrayView::renderColumns( painter, cx, cy, cw, ch );
    // TODO: update non blinking cursors. Should this perhaps be done in the buffercolumn?
    // Then it needs to know about inactive, insideByte and the like... well...
    // perhaps subclassing the buffer columns even more, to CharByteArrayColumnRenderer and ValueByteArrayColumnRenderer?

    if( q->visibleLines(KPixelYs::fromWidth(cy,ch)).includes(mTableCursor->line()) )
    {
        drawActiveCursor( painter );
        drawInactiveCursor( painter );
    }
}

void ByteArrayRowViewPrivate::updateChanged()
{
    Q_Q( ByteArrayRowView );

    const KPixelXs Xs = KPixelXs::fromWidth( q->xOffset(), q->visibleWidth() );

    // collect affected buffer columns
    QList<ByteArrayRowColumnRenderer*> dirtyColumns;

    if( mByteArrayColumn->overlaps(Xs) )
    {
        dirtyColumns.append( mByteArrayColumn );
        mByteArrayColumn->prepareRendering( Xs );
    }

    // any colums to paint?
    if( dirtyColumns.size() > 0 )
    {
        KPixelYs Ys = KPixelYs::fromWidth( q->yOffset(), q->visibleHeight() );

        // calculate affected lines/indizes
        const KHE::Section fullPositions( 0, mTableLayout->noOfBytesPerLine()-1 );
        CoordRange visibleRange( fullPositions, q->visibleLines(Ys) );

        const int lineHeight = q->lineHeight();
        const int xOffset = q->xOffset();
        CoordRange changedRange;
        // as there might be multiple selections on this line redo until no more is changed
        while( hasChanged(visibleRange,&changedRange) )
        {
            KPixelY cy = changedRange.start().line() * lineHeight - q->yOffset();

            QListIterator<ByteArrayRowColumnRenderer*> columnIt( dirtyColumns );
            // only one line?
            if( changedRange.start().line() == changedRange.end().line() )
            {
                const KHE::Section changedPositions( changedRange.start().pos(), changedRange.end().pos() );
                while( columnIt.hasNext() )
                {
                    const KPixelXs xPixels = columnIt.next()->xsOfLinePositionsInclSpaces( changedPositions );

                    q->viewport()->update( xPixels.start()-xOffset, cy, xPixels.width(), lineHeight );
                }
            }
            //
            else
            {
                // first line
                const KHE::Section firstChangedPositions( changedRange.start().pos(), fullPositions.end() );
                while( columnIt.hasNext() )
                {
                    const KPixelXs XPixels = columnIt.next()->xsOfLinePositionsInclSpaces( firstChangedPositions );

                    q->viewport()->update( XPixels.start()-xOffset, cy, XPixels.width(), lineHeight );
                }

                // at least one full line?
                for( int l = changedRange.start().line()+1; l < changedRange.end().line(); ++l )
                {
                    cy += lineHeight;
                    columnIt.toFront();
                    while( columnIt.hasNext() )
                    {
                        const KPixelXs XPixels = columnIt.next()->xsOfLinePositionsInclSpaces( fullPositions );

                        q->viewport()->update( XPixels.start()-xOffset, cy, XPixels.width(), lineHeight );
                    }
                }
                // last line
                cy += lineHeight;
                columnIt.toFront();
                const KHE::Section lastChangedPositions( fullPositions.start(), changedRange.end().pos() );
                while( columnIt.hasNext() )
                {
                    const KPixelXs XPixels = columnIt.next()->xsOfLinePositionsInclSpaces( lastChangedPositions );

                    q->viewport()->update( XPixels.start()-xOffset, cy, XPixels.width(), lineHeight );
                }
            }

            // continue the search at the overnext index
            visibleRange.setStart( changedRange.end()+1 ); //+2 ); TODO: currently bounding ranges are not merged
            if( !visibleRange.isValid() )
                break;
        }
    }

    mTableRanges->resetChangedRanges();
}



void ByteArrayRowViewPrivate::ensureCursorVisible()
{
//   // Not visible or the user is dragging the window, so don't position to caret yet
//   if ( !isVisible() || isHorizontalSliderPressed() || isVerticalSliderPressed() )
//   {
//     d->ensureCursorVisibleInShowEvent = true;
//     return;
//   }
  //static const int Margin = 10;
    ensureVisible( *mByteArrayColumn, mTableCursor->coord() );
}

void ByteArrayRowViewPrivate::ensureVisible( const ByteArrayRowColumnRenderer& column, const Coord& coord )
{
    Q_Q( ByteArrayRowView );

    // TODO: add getCursorRect to BufferColumn
    const KPixelXs cursorXs = KPixelXs::fromWidth( column.xOfLinePosition(coord.pos()),
                                                   column.byteWidth() );

    const KPixelYs cursorYs = KPixelYs::fromWidth( q->lineHeight()*coord.line(), q->lineHeight() );

    const KPixelXs visibleXs = KPixelXs::fromWidth( q->xOffset(), q->visibleWidth() );
    const KPixelYs visibleYs = KPixelXs::fromWidth( q->yOffset(), q->visibleHeight() );

    q->horizontalScrollBar()->setValue( visibleXs.startForInclude(cursorXs) );
    q->verticalScrollBar()->setValue( visibleYs.startForInclude(cursorYs) );
}


#if 0
void ByteArrayRowViewPrivate::clipboardChanged()
{
    Q_Q( ByteArrayRowView );

    // don't listen to selection changes
    q->disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()) );
    selectAll( false );
}
#endif

void ByteArrayRowViewPrivate::mousePressEvent( QMouseEvent* mouseEvent )
{
    Q_Q( ByteArrayRowView );

    pauseCursor();
    mValueEditor->finishEdit();

    // care about a left button press?
    if( mouseEvent->button() == Qt::LeftButton )
    {
        mMousePressed = true;

        // select whole line?
        if( mTrippleClickTimer->isActive()
            && (mouseEvent->globalPos()-mDoubleClickPoint).manhattanLength() < QApplication::startDragDistance() )
        {
            mTrippleClickTimer->stop();
            const int indexAtFirstDoubleClickLinePosition = mTableLayout->indexAtFirstLinePosition( mDoubleClickLine );
            mTableRanges->setSelectionStart( indexAtFirstDoubleClickLinePosition );
            mTableCursor->gotoIndex( indexAtFirstDoubleClickLinePosition );
            mTableCursor->gotoLineEnd();
            mTableRanges->setSelectionEnd( cursorPosition() );
            updateChanged();

            unpauseCursor();
            emit q->cursorPositionChanged( cursorPosition() );
            return;
        }

        const QPoint mousePoint = q->viewportToColumns( mouseEvent->pos() );

        // start of a drag perhaps?
        if( mTableRanges->hasSelection() && mTableRanges->selectionIncludes(indexByPoint( mousePoint )) )
        {
            mDragStartPossible = true;
            mDragStartTimer->start( QApplication::startDragTime() );
            mDragStartPoint = mousePoint;
        }
        else
        {
            placeCursor( mousePoint );
            ensureCursorVisible();

            const int realIndex = mTableCursor->realIndex();
            if( mTableRanges->selectionStarted() )
            {
                if( mouseEvent->modifiers() & Qt::SHIFT )
                    mTableRanges->setSelectionEnd( realIndex );
                else
                {
                    mTableRanges->removeSelection();
                    mTableRanges->setSelectionStart( realIndex );
                }
            }
            else // start of a new selection possible
            {
                mTableRanges->setSelectionStart( realIndex );

                if( !isEffectiveReadOnly() && (mouseEvent->modifiers()&Qt::SHIFT) ) // TODO: why only for readwrite?
                    mTableRanges->setSelectionEnd( realIndex );
            }

            mTableRanges->removeFurtherSelections();
        }
    }
    else if( mouseEvent->button() == Qt::MidButton )
        mTableRanges->removeSelection();

    if( mTableRanges->isModified() )
    {
        updateChanged();
        q->viewport()->setCursor( isEffectiveReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor );
    }

    unpauseCursor();
}


void ByteArrayRowViewPrivate::mouseMoveEvent( QMouseEvent *mouseEvent )
{
    Q_Q( ByteArrayRowView );

    const QPoint movePoint = q->viewportToColumns( mouseEvent->pos() );

    if( mMousePressed )
    {
        if( mDragStartPossible )
        {
            mDragStartTimer->stop();
            // moved enough for a drag?
            if( (movePoint-mDragStartPoint).manhattanLength() > QApplication::startDragDistance() )
                startDrag();
            if( !isEffectiveReadOnly() )
                q->viewport()->setCursor( Qt::IBeamCursor );
            return;
        }
        // selecting
        handleMouseMove( movePoint );
    }
    else if( !isEffectiveReadOnly() )
    {
        // visual feedback for possible dragging
        const bool InSelection =
            mTableRanges->hasSelection() && mTableRanges->selectionIncludes( indexByPoint(movePoint) );
        q->viewport()->setCursor( InSelection?Qt::ArrowCursor:Qt::IBeamCursor );
    }
}


void ByteArrayRowViewPrivate::mouseReleaseEvent( QMouseEvent* mouseEvent )
{
    Q_Q( ByteArrayRowView );

    const QPoint releasePoint = q->viewportToColumns( mouseEvent->pos() );

    // this is not the release of a doubleclick so we need to process it?
    if( !mInDoubleClick )
    {
        const int line = q->lineAt( releasePoint.y() );
        const int pos = mByteArrayColumn->linePositionOfX( releasePoint.x() ); // TODO: can we be sure here about the active column?
        const int index = mTableLayout->indexAtCCoord( Coord(pos,line) ); // TODO: can this be another index than the one of the cursor???
        emit q->clicked( index );
    }

    if( mMousePressed )
    {
        mMousePressed = false;

        if( mScrollTimer->isActive() )
            mScrollTimer->stop();

        // was only click inside selection, nothing dragged?
        if( mDragStartPossible )
        {
            selectAll( false );
            mDragStartTimer->stop();
            mDragStartPossible = false;

            placeCursor( mDragStartPoint );
            ensureCursorVisible();

            unpauseCursor();
        }
        // was end of selection operation?
        else if( mTableRanges->hasSelection() )
        {
            if( QApplication::clipboard()->supportsSelection() )
            {
                mClipboardMode = QClipboard::Selection;
                q->disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()) );

                copy();

                //TODO: why did we do this? And why does the disconnect above not work?
                // got connected multiple times after a few selections by mouse
        //         connect( QApplication::clipboard(), SIGNAL(selectionChanged()), SLOT(clipboardChanged()) );
                mClipboardMode = QClipboard::Clipboard;
            }
        }
    }
    // middle mouse button paste?
    else if( mouseEvent->button() == Qt::MidButton && !isEffectiveReadOnly() )
    {
        pauseCursor();
        mValueEditor->finishEdit();

        placeCursor( releasePoint );

        // replace no selection?
        if( mTableRanges->hasSelection() && !mTableRanges->selectionIncludes(mTableCursor->index()) )
            mTableRanges->removeSelection();

        mClipboardMode = QClipboard::Selection;
        paste();
        mClipboardMode = QClipboard::Clipboard;

        // ensure selection changes to be drawn TODO: create a insert/pasteAtCursor that leaves out drawing
        updateChanged();

        ensureCursorVisible();
        unpauseCursor();
    }

    emit q->cursorPositionChanged( cursorPosition() );

    mInDoubleClick = false;

    if( mTableRanges->selectionJustStarted() )
        mTableRanges->removeSelection();

    if( !mOverWrite ) emit q->cutAvailable( mTableRanges->hasSelection() );
    emit q->copyAvailable( mTableRanges->hasSelection() );
    emit q->selectionChanged( mTableRanges->hasSelection() );
}


// gets called after press and release instead of a plain press event (?)
void ByteArrayRowViewPrivate::mouseDoubleClickEvent( QMouseEvent* mouseEvent )
{
    Q_Q( ByteArrayRowView );

    // we are only interested in LMB doubleclicks
    if( mouseEvent->button() != Qt::LeftButton )
    {
        mouseEvent->ignore();
        return;
    }

    mDoubleClickLine = mTableCursor->line();

    const int index = mTableCursor->validIndex();

    if( mActiveCoding == AbstractByteArrayView::CharCodingId )
    {
        selectWord( index );

        // as we already have a doubleclick maybe it is a tripple click
        mTrippleClickTimer->start( qApp->doubleClickInterval() );
        mDoubleClickPoint = mouseEvent->globalPos();
    }
    //  else
    //    mValueEditor->goInsideByte(); TODO: make this possible again

    mInDoubleClick = true; //
    mMousePressed = true;

    emit q->doubleClicked( index );
}


void ByteArrayRowViewPrivate::autoScrollTimerDone()
{
    Q_Q( ByteArrayRowView );

    if( mMousePressed )
        handleMouseMove( q->viewportToColumns(q->viewport()->mapFromGlobal( QCursor::pos() )) );
}


void ByteArrayRowViewPrivate::handleMouseMove( const QPoint& point ) // handles the move of the mouse with pressed buttons
{
    Q_Q( ByteArrayRowView );

    const int yOffset = q->yOffset();
    const int behindLastYOffset = yOffset + q->visibleHeight();
    // scrolltimer but inside of viewport?
    if( mScrollTimer->isActive() )
    {
        if( yOffset <= point.y() && point.y() < behindLastYOffset )
            mScrollTimer->stop();
    }
    // no scrolltimer and outside of viewport?
    else
    {
        if( point.y() < yOffset || behindLastYOffset <= point.y() )
            mScrollTimer->start( DefaultScrollTimerPeriod );
    }
    pauseCursor();

    placeCursor( point );
    ensureCursorVisible();

    // do wordwise selection?
    if( mInDoubleClick && mTableRanges->hasFirstWordSelection() )
    {
        int newIndex = mTableCursor->realIndex();
        const KHE::Section firstWordSelection = mTableRanges->firstWordSelection();
        const KHECore::WordByteArrayService WBS( mByteArrayModel, charCodec() );
        // are we before the selection?
        if( firstWordSelection.startsBehind(newIndex) )
        {
            mTableRanges->ensureWordSelectionForward( false );
            newIndex = WBS.indexOfLeftWordSelect( newIndex );
        }
        // or behind?
        else if( firstWordSelection.endsBefore(newIndex) )
        {
            mTableRanges->ensureWordSelectionForward( true );
            newIndex = WBS.indexOfRightWordSelect( newIndex );
        }
        // or inside?
        else
        {
            mTableRanges->ensureWordSelectionForward( true );
            newIndex = firstWordSelection.nextBehindEnd();
        }

        mTableCursor->gotoIndex( newIndex );
    }

    if( mTableRanges->selectionStarted() )
        mTableRanges->setSelectionEnd( cursorPosition() );

    updateChanged();

    unpauseCursor();
    emit q->cursorPositionChanged( cursorPosition() );
}


void ByteArrayRowViewPrivate::startDrag()
{
    Q_Q( ByteArrayRowView );

    // reset states
    mMousePressed = false;
    mInDoubleClick = false;
    mDragStartPossible = false;

    // create data
    QMimeData *dragData = selectionAsMimeData();
    if( !dragData )
        return;

    QDrag *drag = new QDrag( q );
    drag->setMimeData( dragData );

    Qt::DropActions request = (isEffectiveReadOnly()||mOverWrite) ? Qt::CopyAction : Qt::CopyAction|Qt::MoveAction;
    Qt::DropAction dropAction = drag->exec( request );

    if( dropAction == Qt::MoveAction )
        // Not inside this widget itself?
        if( drag->target() != q )
            removeSelectedData();
}

ByteArrayRowViewPrivate::~ByteArrayRowViewPrivate()
{
    delete mCursorPixmaps;
}

}
