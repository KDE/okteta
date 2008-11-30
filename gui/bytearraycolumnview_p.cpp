/*
    This file is part of the Okteta Gui library, part of the KDE project.

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

#include "bytearraycolumnview_p.h"

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


ByteArrayColumnViewPrivate::ByteArrayColumnViewPrivate( ByteArrayColumnView* parent )
: AbstractByteArrayViewPrivate( parent ),
   mCursorPixmaps( new KCursor() ),
   mMousePressed( false ),
   mInDoubleClick( false ),
   mInDnD( false ),
   mDragStartPossible( false ),
   mBlinkCursorVisible( false )
{
}

void ByteArrayColumnViewPrivate::init()
{
    Q_Q( ByteArrayColumnView );

    mCursorBlinkTimer = new QTimer( q );
    mScrollTimer = new QTimer( q );
    mDragStartTimer = new QTimer( q );
    mTrippleClickTimer = new QTimer( q );

    // creating the columns in the needed order
    mOffsetColumn =
        new OffsetColumnRenderer( q, mTableLayout, KOffsetFormat::Hexadecimal );
    mFirstBorderColumn =
        new BorderColumnRenderer( q, false );
    mValueColumn =
        new ValueByteArrayColumnRenderer( q, mByteArrayModel, mTableLayout, mTableRanges );
    mSecondBorderColumn =
        new BorderColumnRenderer( q, true );
    mCharColumn =
        new CharByteArrayColumnRenderer( q, mByteArrayModel, mTableLayout, mTableRanges );

    // select the active column
    mActiveColumn = mCharColumn;
    mInactiveColumn = mValueColumn;

    // set char encoding
    mValueColumn->setValueCodec( (KHECore::ValueCoding)mValueCoding, mValueCodec );
    mValueColumn->setCharCodec( mCharCodec );
    mCharColumn->setCharCodec( mCharCodec );

    adaptController();

    q->setFont( KGlobalSettings::fixedFont() );

    q->connect( mCursorBlinkTimer, SIGNAL(timeout()), q, SLOT(blinkCursor()) );
    q->connect( mScrollTimer,      SIGNAL(timeout()), q, SLOT(autoScrollTimerDone()) );
    q->connect( mDragStartTimer,   SIGNAL(timeout()), q, SLOT(startDrag()) );
    mDragStartTimer->setSingleShot( true );
    mTrippleClickTimer->setSingleShot( true );

    q->setAcceptDrops( true );
}

AbstractByteArrayView::CodingTypeId ByteArrayColumnViewPrivate::activeCoding() const
{
    const bool isValueColumnActive = ( mActiveColumn == (AbstractByteArrayColumnRenderer*)mValueColumn );
    return isValueColumnActive ? AbstractByteArrayView::ValueCodingId : AbstractByteArrayView::CharCodingId;
}

int ByteArrayColumnViewPrivate::visibleCodings() const
{
    return (mValueColumn->isVisible() ? AbstractByteArrayView::ValueCodingId : 0)
           | (mCharColumn->isVisible() ? AbstractByteArrayView::CharCodingId : 0);
}

void ByteArrayColumnViewPrivate::setByteArrayModel( KHECore::AbstractByteArrayModel* _byteArrayModel )
{
    mValueEditor->reset();

    // TODO: this fails if _byteArrayModel == null
    mValueColumn->set( _byteArrayModel );
    mCharColumn->set( _byteArrayModel );

    AbstractByteArrayViewPrivate::setByteArrayModel( _byteArrayModel );
}


void ByteArrayColumnViewPrivate::setBufferSpacing( KPixelX ByteSpacing, int noOfGroupedBytes, KPixelX GroupSpacing )
{
    if( !mValueColumn->setSpacing(ByteSpacing,noOfGroupedBytes,GroupSpacing) )
        return;

    updateViewByWidth();
}


void ByteArrayColumnViewPrivate::setValueCoding( AbstractByteArrayView::ValueCoding valueCoding )
{
    Q_Q( ByteArrayColumnView );

    if( mValueCoding == valueCoding )
        return;

    const uint oldCodingWidth = mValueCodec->encodingWidth();

    AbstractByteArrayViewPrivate::setValueCoding( valueCoding );

    mValueColumn->setValueCodec( (KHECore::ValueCoding)mValueCoding, mValueCodec );
    mValueEditor->adaptToValueCodecChange();

    const uint newCodingWidth = mValueCodec->encodingWidth();

    // no change in the width?
    if( newCodingWidth == oldCodingWidth )
        q->updateColumn( *mValueColumn );
    else
        updateViewByWidth();

    emit q->valueCodingChanged( valueCoding );
}


void ByteArrayColumnViewPrivate::setByteSpacingWidth( int/*KPixelX*/ byteSpacingWidth )
{
    if( !mValueColumn->setByteSpacingWidth(byteSpacingWidth) )
        return;
    updateViewByWidth();
}

void ByteArrayColumnViewPrivate::setNoOfGroupedBytes( int noOfGroupedBytes )
{
    if( !mValueColumn->setNoOfGroupedBytes(noOfGroupedBytes) )
        return;
    updateViewByWidth();
}


void ByteArrayColumnViewPrivate::setGroupSpacingWidth( int/*KPixelX*/ groupSpacingWidth )
{
    if( !mValueColumn->setGroupSpacingWidth(groupSpacingWidth) )
        return;
    updateViewByWidth();
}


void ByteArrayColumnViewPrivate::setBinaryGapWidth( int/*KPixelX*/ binaryGapWidth )
{
    if( !mValueColumn->setBinaryGapWidth(binaryGapWidth) )
        return;
    updateViewByWidth();
}


void ByteArrayColumnViewPrivate::setSubstituteChar( QChar substituteChar )
{
    Q_Q( ByteArrayColumnView );

    if( !mCharColumn->setSubstituteChar(substituteChar) )
        return;
    pauseCursor();
    q->updateColumn( *mCharColumn );
    unpauseCursor();
}

void ByteArrayColumnViewPrivate::setUndefinedChar( QChar undefinedChar )
{
    Q_Q( ByteArrayColumnView );

    if( !mCharColumn->setUndefinedChar(undefinedChar) )
        return;
    pauseCursor();
    q->updateColumn( *mCharColumn );
    unpauseCursor();
}

void ByteArrayColumnViewPrivate::setShowsNonprinting( bool showingNonprinting )
{
    Q_Q( ByteArrayColumnView );

    if( !mCharColumn->setShowingNonprinting(showingNonprinting) )
        return;
    pauseCursor();
    q->updateColumn( *mCharColumn );
    unpauseCursor();
}


void ByteArrayColumnViewPrivate::setCharCoding( AbstractByteArrayView::CharCoding charCoding )
{
    Q_Q( ByteArrayColumnView );

    if( mCharCoding == charCoding )
        return;

    AbstractByteArrayViewPrivate::setCharCoding( charCoding );

    pauseCursor();

    mValueColumn->setCharCodec( mCharCodec );
    mCharColumn->setCharCodec( mCharCodec );

    q->updateColumn( *mValueColumn );
    q->updateColumn( *mCharColumn );

    unpauseCursor();

    emit q->charCodecChanged( charCodingName() );
}

// TODO: join with function above!
void ByteArrayColumnViewPrivate::setCharCoding( const QString& newCharCodingName )
{
    Q_Q( ByteArrayColumnView );

    if( charCodingName() == newCharCodingName )
        return;

    AbstractByteArrayViewPrivate::setCharCoding( newCharCodingName );

    pauseCursor();

    mValueColumn->setCharCodec( mCharCodec );
    mCharColumn->setCharCodec( mCharCodec );

    q->updateColumn( *mValueColumn );
    q->updateColumn( *mCharColumn );

    unpauseCursor();

    emit q->charCodecChanged( charCodingName() );
}

void ByteArrayColumnViewPrivate::setByteTypeColored( bool isColored )
{
    Q_Q( ByteArrayColumnView );

    if( isColored == mValueColumn->isByteTypeColored() )
        return;

    mValueColumn->setByteTypeColored( isColored );
    mCharColumn->setByteTypeColored( isColored );

    pauseCursor();
    q->updateColumn( *mValueColumn );
    q->updateColumn( *mCharColumn );
    unpauseCursor();
}


void ByteArrayColumnViewPrivate::handleFontChange( const QFont& oldFont )
{
    Q_Q( ByteArrayColumnView );

    q->AbstractByteArrayView::fontChange( oldFont );

    // get new values
    const QFontMetrics newFontMetrics = q->fontMetrics();
    KPixelX digitWidth = newFontMetrics.maxWidth();
    KPixelY digitBaseLine = newFontMetrics.ascent();

     q->setLineHeight( newFontMetrics.height() );

    // update all dependant structures
    mTableLayout->setNoOfLinesPerPage( q->noOfLinesPerPage() );

    mOffsetColumn->setMetrics( digitWidth, digitBaseLine );
    mValueColumn->setMetrics( digitWidth, digitBaseLine );
    mCharColumn->setMetrics( digitWidth, digitBaseLine );

    updateViewByWidth();
}


void ByteArrayColumnViewPrivate::adjustToLayoutNoOfBytesPerLine()
{
    Q_Q( ByteArrayColumnView );

    mValueColumn->resetXBuffer();
    mCharColumn->resetXBuffer();

    q->updateWidths();
}


void ByteArrayColumnViewPrivate::toggleOffsetColumn( bool showOffsetColumn )
{
    const bool isVisible = mOffsetColumn->isVisible();
    // no change?
    if( isVisible == showOffsetColumn )
        return;

    mOffsetColumn->setVisible( showOffsetColumn );

    updateViewByWidth();
}


QSize ByteArrayColumnViewPrivate::minimumSizeHint() const
{
    Q_Q( const ByteArrayColumnView );

    // TODO: better minimal width (visibility!)
    return QSize(
        mOffsetColumn->visibleWidth()
        + mFirstBorderColumn->visibleWidth()
        + mSecondBorderColumn->visibleWidth()
        + mValueColumn->byteWidth()
        + mCharColumn->byteWidth(),
        q->lineHeight()
        + q->noOfLines()>1? q->style()->pixelMetric(QStyle::PM_ScrollBarExtent):0 );
}


int ByteArrayColumnViewPrivate::fittingBytesPerLine() const
{
    Q_Q( const ByteArrayColumnView );

    const QSize newSize = q->maximumViewportSize();
    const KPixelX reservedWidth =
        mOffsetColumn->visibleWidth()
        + mFirstBorderColumn->visibleWidth()
        + mSecondBorderColumn->visibleWidth();

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
    const KPixelX digitWidth = mValueColumn->digitWidth();
    const KPixelX charByteWidth = mCharColumn->isVisible() ? digitWidth : 0;
    const KPixelX valueByteWidth = mValueColumn->isVisible() ? mValueColumn->byteWidth() : 0;
    const KPixelX byteSpacingWidth = mValueColumn->isVisible() ? mValueColumn->byteSpacingWidth() : 0;
    KPixelX groupSpacingWidth;
    int noOfGroupedBytes = mValueColumn->noOfGroupedBytes();
    // no grouping?
    if( noOfGroupedBytes == 0 )
    {
        // faking grouping by 1
        noOfGroupedBytes = 1;
        groupSpacingWidth = 0;
    }
    else
        groupSpacingWidth = mValueColumn->isVisible() ? mValueColumn->groupSpacingWidth() : 0;

    const KPixelX valueByteGroupWidth =  noOfGroupedBytes * valueByteWidth + (noOfGroupedBytes-1)*byteSpacingWidth;
    const KPixelX charByteGroupWidth = noOfGroupedBytes * charByteWidth;
    const KPixelX totalGroupWidth = valueByteGroupWidth + groupSpacingWidth + charByteGroupWidth;

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

//         kDebug() << "Left: " << availableWidth << "("<<valueByteWidth<<", "<<charByteWidth<<")" ;

            if( availableWidth > 0 )
                fittingBytesPerLine += (availableWidth+byteSpacingWidth) / (valueByteWidth+byteSpacingWidth+charByteWidth);

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
//              << ", h:" << valueByteGroupWidth
//              << ", t:" << charByteGroupWidth
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

void ByteArrayColumnViewPrivate::setVisibleCodings( int newColumns )
{
    const int oldColumns = visibleCodings();

    // no changes or no column selected?
    if( newColumns == oldColumns || !(newColumns&AbstractByteArrayView::BothCodingsId) )
        return;

    mValueColumn->setVisible( AbstractByteArrayView::ValueCodingId & newColumns );
    mCharColumn->setVisible( AbstractByteArrayView::CharCodingId & newColumns );
    mSecondBorderColumn->setVisible( newColumns == AbstractByteArrayView::BothCodingsId );

    // active column not visible anymore?
    if( !mActiveColumn->isVisible() )
    {
        AbstractByteArrayColumnRenderer* h = mActiveColumn;
        mActiveColumn = mInactiveColumn;
        mInactiveColumn = h;
        adaptController();
    }

    updateViewByWidth();
}


void ByteArrayColumnViewPrivate::setActiveCoding( AbstractByteArrayView::CodingTypeId codingId )
{
    // no changes or not visible?
    if( codingId == activeCoding()
        || (codingId == AbstractByteArrayView::ValueCodingId && !mValueColumn->isVisible())
        || (codingId == AbstractByteArrayView::CharCodingId && !mCharColumn->isVisible()) )
        return;

    pauseCursor();
    mValueEditor->finishEdit();

    if( codingId == AbstractByteArrayView::ValueCodingId )
    {
        mActiveColumn = mValueColumn;
        mInactiveColumn = mCharColumn;
    }
    else
    {
        mActiveColumn = mCharColumn;
        mInactiveColumn = mValueColumn;
    }
    adaptController();

    ensureCursorVisible();
    unpauseCursor();
}


void ByteArrayColumnViewPrivate::placeCursor( const QPoint& point )
{
    Q_Q( ByteArrayColumnView );

    // switch active column if needed
    if( mCharColumn->isVisible() && point.x() >= mCharColumn->x() )
    {
        mActiveColumn = mCharColumn;
        mInactiveColumn = mValueColumn;
    }
    else
    {
        mActiveColumn = mValueColumn;
        mInactiveColumn = mCharColumn;
    }
    adaptController();

    // get coord of click and whether this click was closer to the end of the pos
    const Coord coord( mActiveColumn->magneticLinePositionOfX(point.x()), q->lineAt(point.y()) );

    mTableCursor->gotoCCoord( coord );
    emit q->cursorPositionChanged( cursorPosition() );
}


int ByteArrayColumnViewPrivate::indexByPoint( const QPoint& point ) const
{
    Q_Q( const ByteArrayColumnView );

    const AbstractByteArrayColumnRenderer* column =
         ( mCharColumn->isVisible() && point.x() >= mCharColumn->x() ) ?
         (AbstractByteArrayColumnRenderer *)mCharColumn : (AbstractByteArrayColumnRenderer *)mValueColumn;

    const Coord coord( column->linePositionOfX(point.x()), q->lineAt(point.y()) );

    return mTableLayout->indexAtCCoord( coord );
}


void ByteArrayColumnViewPrivate::blinkCursor()
{
    // skip the cursor drawing?
    if( mCursorPaused || mValueEditor->isInEditMode() )
        return;

    // switch the cursor state
    mBlinkCursorVisible = !mBlinkCursorVisible;
    updateCursor( *mActiveColumn );
}


void ByteArrayColumnViewPrivate::startCursor()
{
    mCursorPaused = false;

    updateCursors();

    mCursorBlinkTimer->start( QApplication::cursorFlashTime()/2 );
}


void ByteArrayColumnViewPrivate::unpauseCursor()
{
    mCursorPaused = false;

    if( mCursorBlinkTimer->isActive() )
        updateCursors();
}


void ByteArrayColumnViewPrivate::updateCursors()
{
    createCursorPixmaps();

    mBlinkCursorVisible = true;
    updateCursor( *mActiveColumn );
    updateCursor( *mInactiveColumn );
}


void ByteArrayColumnViewPrivate::stopCursor()
{
    mCursorBlinkTimer->stop();

    pauseCursor();
}


void ByteArrayColumnViewPrivate::pauseCursor()
{
    mCursorPaused = true;

    mBlinkCursorVisible = false;
    updateCursor( *mActiveColumn );
    updateCursor( *mInactiveColumn );
}

void ByteArrayColumnViewPrivate::updateCursor( const AbstractByteArrayColumnRenderer& column )
{
    Q_Q( ByteArrayColumnView );

    const int x = column.xOfLinePosition( mTableCursor->pos() ) - q->xOffset();
    const int y = q->lineHeight() * mTableCursor->line() - q->yOffset();
    const int w = column.byteWidth();

    q->viewport()->update( x,y, w,q->lineHeight() );
}

void ByteArrayColumnViewPrivate::createCursorPixmaps()
{
    Q_Q( ByteArrayColumnView );

    // create mCursorPixmaps
    mCursorPixmaps->setSize( mActiveColumn->byteWidth(), q->lineHeight() );

    const int index = mTableCursor->validIndex();

    QPainter painter;
    painter.begin( &mCursorPixmaps->offPixmap() );
    painter.initFrom( q );
    mActiveColumn->renderByte( &painter, index );
    painter.end();

    painter.begin( &mCursorPixmaps->onPixmap() );
    painter.initFrom( q );
    mActiveColumn->renderCursor( &painter, index );
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


void ByteArrayColumnViewPrivate::drawActiveCursor( QPainter *painter )
{
    Q_Q( ByteArrayColumnView );

    // any reason to skip the cursor drawing?
    if( mBlinkCursorVisible && !q->hasFocus() && !q->viewport()->hasFocus() && !mDropper->isActive() )
        return;

    const int x = mActiveColumn->xOfLinePosition( mTableCursor->pos() );
    const int y = q->lineHeight() * mTableCursor->line();

    painter->translate( x, y );

    // paint edited byte?
    if( mValueEditor->isInEditMode() )
    {
        const int index = mTableCursor->index();

        if( mBlinkCursorVisible )
            mValueColumn->renderEditedByte( painter, mValueEditor->value(), mValueEditor->valueAsString() );
        else
            mValueColumn->renderByte( painter, index );
    }
    else
        painter->drawPixmap( mCursorPixmaps->cursorX(), 0,
                            mBlinkCursorVisible?mCursorPixmaps->onPixmap():mCursorPixmaps->offPixmap(),
                            mCursorPixmaps->cursorX(),0,mCursorPixmaps->cursorW(),-1 );

    painter->translate( -x, -y );
}


void ByteArrayColumnViewPrivate::drawInactiveCursor( QPainter *painter )
{
    Q_Q( ByteArrayColumnView );

    // any reason to skip the cursor drawing?
    if( !mInactiveColumn->isVisible()
        || mCursorPaused
        || (!mCursorPaused && !q->hasFocus() && !q->viewport()->hasFocus() && !mDropper->isActive())  )
        return;

    const int index = mTableCursor->validIndex();

    const int x = mInactiveColumn->xOfLinePosition( mTableCursor->pos() );
    const int y = q->lineHeight() * mTableCursor->line();

    painter->translate( x, y );

    const AbstractByteArrayColumnRenderer::FrameStyle frameStyle =
        mTableCursor->isBehind() ?                    AbstractByteArrayColumnRenderer::Right :
        (mOverWrite||mValueEditor->isInEditMode()) ? AbstractByteArrayColumnRenderer::Frame :
                                                     AbstractByteArrayColumnRenderer::Left;
    mInactiveColumn->renderFramedByte( painter, index, frameStyle );

    painter->translate( -x, -y );
}


void ByteArrayColumnViewPrivate::renderColumns( QPainter* painter, int cx, int cy, int cw, int ch )
{
    Q_Q( ByteArrayColumnView );

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

void ByteArrayColumnViewPrivate::updateChanged()
{
    Q_Q( ByteArrayColumnView );

    const int xOffset = q->xOffset();
    const KPixelXs Xs = KPixelXs::fromWidth( xOffset, q->visibleWidth() );

    // do updates in offset column 
    const KHE::Section changedOffsetLines = mTableRanges->changedOffsetLines();
    if( !changedOffsetLines.isEmpty() )
        q->updateColumn( *mOffsetColumn, changedOffsetLines );

    // collect affected buffer columns
    QList<AbstractByteArrayColumnRenderer*> dirtyColumns;

    AbstractByteArrayColumnRenderer *column = mValueColumn;
    while( true )
    {
        if( column->isVisible() && column->overlaps(Xs) )
        {
            dirtyColumns.append( column );
            column->prepareRendering( Xs );
        }

        if( column == mCharColumn )
            break;
        column = mCharColumn;
    }

    // any colums to paint?
    if( dirtyColumns.size() > 0 )
    {
        // calculate affected lines/indizes
        const KHE::Section fullPositions( 0, mTableLayout->noOfBytesPerLine()-1 );
        CoordRange visibleRange( fullPositions, q->visibleLines() );

        const int lineHeight = q->lineHeight();
        CoordRange changedRange;
        // as there might be multiple selections on this line redo until no more is changed
        while( hasChanged(visibleRange,&changedRange) )
        {
            KPixelY cy = q->yOffsetOfLine( changedRange.start().line() );

            QListIterator<AbstractByteArrayColumnRenderer*> columnIt( dirtyColumns );
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



void ByteArrayColumnViewPrivate::ensureCursorVisible()
{
//   // Not visible or the user is dragging the window, so don't position to caret yet
//   if ( !isVisible() || isHorizontalSliderPressed() || isVerticalSliderPressed() )
//   {
//     d->ensureCursorVisibleInShowEvent = true;
//     return;
//   }
  //static const int Margin = 10;
    ensureVisible( *mActiveColumn, mTableCursor->coord() );
}

void ByteArrayColumnViewPrivate::ensureVisible( const AbstractByteArrayColumnRenderer& column, const Coord& coord )
{
    Q_Q( ByteArrayColumnView );

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
void ByteArrayColumnViewPrivate::clipboardChanged()
{
    Q_Q( ByteArrayColumnView );

    // don't listen to selection changes
    q->disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()) );
    selectAll( false );
}
#endif

void ByteArrayColumnViewPrivate::mousePressEvent( QMouseEvent* mouseEvent )
{
    Q_Q( ByteArrayColumnView );

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


void ByteArrayColumnViewPrivate::mouseMoveEvent( QMouseEvent *mouseEvent )
{
    Q_Q( ByteArrayColumnView );

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


void ByteArrayColumnViewPrivate::mouseReleaseEvent( QMouseEvent* mouseEvent )
{
    Q_Q( ByteArrayColumnView );

    const QPoint releasePoint = q->viewportToColumns( mouseEvent->pos() );

    // this is not the release of a doubleclick so we need to process it?
    if( !mInDoubleClick )
    {
        const int line = q->lineAt( releasePoint.y() );
        const int pos = mActiveColumn->linePositionOfX( releasePoint.x() ); // TODO: can we be sure here about the active column?
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
void ByteArrayColumnViewPrivate::mouseDoubleClickEvent( QMouseEvent* mouseEvent )
{
    Q_Q( ByteArrayColumnView );

    // we are only interested in LMB doubleclicks
    if( mouseEvent->button() != Qt::LeftButton )
    {
        mouseEvent->ignore();
        return;
    }

    mDoubleClickLine = mTableCursor->line();

    const int index = mTableCursor->validIndex();

    if( mActiveColumn == mCharColumn )
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


void ByteArrayColumnViewPrivate::autoScrollTimerDone()
{
    Q_Q( ByteArrayColumnView );

    if( mMousePressed )
        handleMouseMove( q->viewportToColumns(q->viewport()->mapFromGlobal( QCursor::pos() )) );
}


void ByteArrayColumnViewPrivate::handleMouseMove( const QPoint& point ) // handles the move of the mouse with pressed buttons
{
    Q_Q( ByteArrayColumnView );

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


void ByteArrayColumnViewPrivate::startDrag()
{
    Q_Q( ByteArrayColumnView );

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

ByteArrayColumnViewPrivate::~ByteArrayColumnViewPrivate()
{
    delete mCursorPixmaps;
}

}
