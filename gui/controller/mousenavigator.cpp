/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "mousenavigator.h"

// lib
#include <abstractbytearrayview.h>
#include <bytearraytableranges.h>
#include <bytearraytablecursor.h>
#include <bytearraytablelayout.h>
#include <wordbytearrayservice.h>
// Qt
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QMouseEvent>
#include <QtCore/QTimer>

#include <KDebug>
namespace Okteta
{
static const int DefaultScrollTimerPeriod = 100;


MouseNavigator::MouseNavigator( AbstractByteArrayView* view, AbstractMouseController* parent )
  : AbstractMouseController( view, parent ),
    mLMBPressed( false ),
    mInLMBDoubleClick( false ),
    mInDnD( false ),
    mDragStartPossible( false )
{
    mScrollTimer = new QTimer( this );
    mDragStartTimer = new QTimer( this );
    mTrippleClickTimer = new QTimer( this );

    connect( mScrollTimer,      SIGNAL(timeout()), SLOT(autoScrollTimerDone()) );
    connect( mDragStartTimer,   SIGNAL(timeout()), SLOT(startDrag()) );
    mDragStartTimer->setSingleShot( true );
    mTrippleClickTimer->setSingleShot( true );
}

bool MouseNavigator::handleMousePressEvent( QMouseEvent* mouseEvent )
{
    bool eventUsed = false;

    if( mouseEvent->button() == Qt::LeftButton )
    {
        ByteArrayTableCursor* tableCursor = mView->tableCursor();
        ByteArrayTableRanges* tableRanges = mView->tableRanges();
        ByteArrayTableLayout* tableLayout = mView->layout();

        const bool oldHasSelection = tableRanges->hasSelection();

        mView->pauseCursor();
        mView->finishByteEdit();

        mLMBPressed = true;

        // select whole line?
        if( mTrippleClickTimer->isActive()
            && (mouseEvent->globalPos()-mDoubleClickPoint).manhattanLength() < QApplication::startDragDistance() )
        {
            mTrippleClickTimer->stop();
            const Address indexAtFirstDoubleClickLinePosition = tableLayout->indexAtFirstLinePosition( mDoubleClickLine );
            tableRanges->setSelectionStart( indexAtFirstDoubleClickLinePosition );
            tableCursor->gotoIndex( indexAtFirstDoubleClickLinePosition );
            tableCursor->gotoLineEnd();
            tableRanges->setSelectionEnd( mView->cursorPosition() );
            mView->updateChanged();

            mView->unpauseCursor();

            const bool newHasSelection = tableRanges->hasSelection();
            emit mView->cursorPositionChanged( mView->cursorPosition() );
            emit mView->selectionChanged( tableRanges->selection() );
            if( oldHasSelection != newHasSelection )
            {
                if( ! mView->isOverwriteMode() ) emit mView->cutAvailable( newHasSelection );
                emit mView->copyAvailable( newHasSelection );
                emit mView->hasSelectedDataChanged( newHasSelection );
            }
        }
        else
        {
            // TODO: pos() is now, not at the moment of the event, use globalPos() for that,.says dox
            const QPoint mousePoint = mView->viewportToColumns( mouseEvent->pos() );

            // start of a drag perhaps?
            if( tableRanges->hasSelection() && tableRanges->selectionIncludes(mView->indexByPoint( mousePoint )) )
            {
                mDragStartPossible = true;
                mDragStartTimer->start( QApplication::startDragTime() );
                mDragStartPoint = mousePoint;
            }
            else
            {
                mView->placeCursor( mousePoint );
                mView->ensureCursorVisible();

                const Address realIndex = tableCursor->realIndex();
                if( tableRanges->selectionStarted() )
                {
                    if( mouseEvent->modifiers() & Qt::SHIFT )
                        tableRanges->setSelectionEnd( realIndex );
                    else
                    {
                        tableRanges->removeSelection();
                        tableRanges->setSelectionStart( realIndex );
                    }
                }
                else // start of a new selection possible
                {
                    tableRanges->setSelectionStart( realIndex );

                    if( !mView->isReadOnly() && (mouseEvent->modifiers()&Qt::SHIFT) ) // TODO: why only for readwrite?
                        tableRanges->setSelectionEnd( realIndex );
                }

                tableRanges->removeFurtherSelections();
            }

            if( tableRanges->isModified() )
            {
                mView->updateChanged();
                mView->viewport()->setCursor( mView->isReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor );
            }

            mView->unpauseCursor();

            const bool newHasSelection = tableRanges->hasSelection();
            emit mView->selectionChanged( tableRanges->selection() );
            if( oldHasSelection != newHasSelection )
            {
                if( ! mView->isOverwriteMode() ) emit mView->cutAvailable( newHasSelection );
                emit mView->copyAvailable( newHasSelection );
                emit mView->hasSelectedDataChanged( newHasSelection );
            }
        }
        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMousePressEvent( mouseEvent );
}

bool MouseNavigator::handleMouseMoveEvent( QMouseEvent* mouseEvent )
{
    bool eventUsed = false;

    if( mouseEvent->buttons() == Qt::LeftButton )
    {
        const QPoint movePoint = mView->viewportToColumns( mouseEvent->pos() );

        if( mLMBPressed )
        {
            if( mDragStartPossible )
            {
                mDragStartTimer->stop();
                // moved enough for a drag?
                if( (movePoint-mDragStartPoint).manhattanLength() > QApplication::startDragDistance() )
                    startDrag();
                if( ! mView->isReadOnly() )
                    mView->viewport()->setCursor( Qt::IBeamCursor );
            }
            else
                // selecting
                handleMouseMove( movePoint );
        }
        else if( ! mView->isReadOnly() )
        {
            ByteArrayTableRanges* tableRanges = mView->tableRanges();

            // visual feedback for possible dragging
            const bool InSelection =
                tableRanges->hasSelection() && tableRanges->selectionIncludes( mView->indexByPoint(movePoint) );
            mView->viewport()->setCursor( InSelection ? Qt::ArrowCursor : Qt::IBeamCursor );
        }
        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMouseMoveEvent( mouseEvent );
}

bool MouseNavigator::handleMouseReleaseEvent( QMouseEvent* mouseEvent )
{
    bool eventUsed = false;

    if( mouseEvent->button() == Qt::LeftButton )
    {
        ByteArrayTableRanges* tableRanges = mView->tableRanges();

        const bool oldHasSelection = tableRanges->hasSelection();
//         const QPoint releasePoint = mView->viewportToColumns( mouseEvent->pos() );

        // this is not the release of a doubleclick so we need to process it?
        if( ! mInLMBDoubleClick )
        {
//             const int line = mView->lineAt( releasePoint.y() );
//             const int pos = mActiveColumn->linePositionOfX( releasePoint.x() ); // TODO: can we be sure here about the active column?
//             const Address index = tableLayout->indexAtCCoord( Coord(pos,line) ); // TODO: can this be another index than the one of the cursor???
//             emit mView->clicked( index ); // TODO: who needs this?
        }

        if( mLMBPressed )
        {
            mLMBPressed = false;

            if( mScrollTimer->isActive() )
                mScrollTimer->stop();

            // was only click inside selection, nothing dragged?
            if( mDragStartPossible )
            {
                mView->selectAll( false );
                mDragStartTimer->stop();
                mDragStartPossible = false;

                mView->placeCursor( mDragStartPoint );
                mView->ensureCursorVisible();

                mView->unpauseCursor();
            }
            // was end of selection operation?
            else if( tableRanges->hasSelection() )
            {
                if( QApplication::clipboard()->supportsSelection() )
                    mView->copyToClipboard( QClipboard::Selection );
            }
        }

        emit mView->cursorPositionChanged( mView->cursorPosition() );

        mInLMBDoubleClick = false;

        if( tableRanges->selectionJustStarted() )
            tableRanges->removeSelection();

        const bool newHasSelection = tableRanges->hasSelection();
        emit mView->selectionChanged( tableRanges->selection() );
        if( oldHasSelection != newHasSelection )
        {
            if( ! mView->isOverwriteMode() ) emit mView->cutAvailable( newHasSelection );
            emit mView->copyAvailable( newHasSelection );
            emit mView->hasSelectedDataChanged( newHasSelection );
        }
        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMouseReleaseEvent( mouseEvent );
}

bool MouseNavigator::handleMouseDoubleClickEvent( QMouseEvent* mouseEvent )
{
    bool eventUsed = false;

    if( mouseEvent->button() == Qt::LeftButton )
    {
        ByteArrayTableCursor* tableCursor = mView->tableCursor();

        mDoubleClickLine = tableCursor->line();

        const Address index = tableCursor->validIndex();

        if( mView->activeCoding() == AbstractByteArrayView::CharCodingId )
        {
            mView->selectWord( index );

            // as we already have a doubleclick maybe it is a tripple click
            mTrippleClickTimer->start( qApp->doubleClickInterval() );
            mDoubleClickPoint = mouseEvent->globalPos();
        }
        //  else
        //    mValueEditor->goInsideByte(); TODO: make this possible again

        mInLMBDoubleClick = true; //
        mLMBPressed = true;

        emit mView->doubleClicked( index );
        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMouseDoubleClickEvent( mouseEvent );
}


void MouseNavigator::autoScrollTimerDone()
{
    if( mLMBPressed )
        handleMouseMove( mView->viewportToColumns(mView->viewport()->mapFromGlobal( QCursor::pos() )) );
}


void MouseNavigator::handleMouseMove( const QPoint& point ) // handles the move of the mouse with pressed buttons
{
    ByteArrayTableCursor* tableCursor = mView->tableCursor();
    ByteArrayTableRanges* tableRanges = mView->tableRanges();

    const bool oldHasSelection = tableRanges->hasSelection();
    const int yOffset = mView->yOffset();
    const int behindLastYOffset = yOffset + mView->visibleHeight();
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
    mView->pauseCursor();

    mView->placeCursor( point );
    mView->ensureCursorVisible();

    // do wordwise selection?
    if( mInLMBDoubleClick && tableRanges->hasFirstWordSelection() )
    {
        Address newIndex = tableCursor->realIndex();
        const AddressRange firstWordSelection = tableRanges->firstWordSelection();
        const WordByteArrayService WBS( mView->byteArrayModel(), mView->charCodec() );
        // are we before the selection?
        if( firstWordSelection.startsBehind(newIndex) )
        {
            tableRanges->ensureWordSelectionForward( false );
            newIndex = WBS.indexOfLeftWordSelect( newIndex );
        }
        // or behind?
        else if( firstWordSelection.endsBefore(newIndex) )
        {
            tableRanges->ensureWordSelectionForward( true );
            newIndex = WBS.indexOfRightWordSelect( newIndex );
        }
        // or inside?
        else
        {
            tableRanges->ensureWordSelectionForward( true );
            newIndex = firstWordSelection.nextBehindEnd();
        }

        tableCursor->gotoIndex( newIndex );
    }

    if( tableRanges->selectionStarted() )
        tableRanges->setSelectionEnd( mView->cursorPosition() );

    mView->updateChanged();

    mView->unpauseCursor();

    const bool newHasSelection = tableRanges->hasSelection();
    emit mView->cursorPositionChanged( mView->cursorPosition() );
    emit mView->selectionChanged( tableRanges->selection() );
    if( oldHasSelection != newHasSelection )
    {
        if( ! mView->isOverwriteMode() ) emit mView->cutAvailable( newHasSelection );
        emit mView->copyAvailable( newHasSelection );
        emit mView->hasSelectedDataChanged( newHasSelection );
    }
}


void MouseNavigator::startDrag()
{
    // reset states
    mLMBPressed = false;
    mInLMBDoubleClick = false;
    mDragStartPossible = false;

    // create data
    QMimeData* dragData = mView->selectionAsMimeData();
    if( ! dragData )
        return;

    QDrag* drag = new QDrag( mView );
    drag->setMimeData( dragData );

    Qt::DropActions request = (mView->isReadOnly()||mView->isOverwriteMode()) ? Qt::CopyAction : Qt::CopyAction|Qt::MoveAction;
    Qt::DropAction dropAction = drag->exec( request );

    if( dropAction == Qt::MoveAction )
    {
        AbstractByteArrayView* targetByteArrayView = qobject_cast<AbstractByteArrayView*>( drag->target() );
        // Not inside this widget itself?
        if( ! targetByteArrayView
            || targetByteArrayView->byteArrayModel() != mView->byteArrayModel() )
            mView->removeSelectedData();
    }
}


MouseNavigator::~MouseNavigator() {}

}
