/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "mousenavigator.hpp"

// lib
#include <abstractbytearrayview.hpp>
#include <abstractbytearrayview_p.hpp>
#include <bytearraytableranges.hpp>
#include <bytearraytablecursor.hpp>
#include <bytearraytablelayout.hpp>
// Okteta core
#include <Okteta/TextByteArrayAnalyzer>
// Qt
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QMouseEvent>
#include <QDrag>

namespace Okteta {
static constexpr int DefaultScrollTimerPeriod = 100;

MouseNavigator::MouseNavigator(AbstractByteArrayViewPrivate* view, AbstractMouseController* parent)
    : AbstractMouseController(view, parent)
    , mLMBPressed(false)
    , mInLMBDoubleClick(false)
    , mDragStartPossible(false)
{
    connect(&mScrollTimer,      &QTimer::timeout, this, &MouseNavigator::autoScrollTimerDone);
    connect(&mDragStartTimer,   &QTimer::timeout, this, &MouseNavigator::startDrag);
    mDragStartTimer.setSingleShot(true);
    mTrippleClickTimer.setSingleShot(true);
}

MouseNavigator::~MouseNavigator() = default;

bool MouseNavigator::handleMousePressEvent(QMouseEvent* mouseEvent)
{
    bool eventUsed = false;

    if (mouseEvent->button() == Qt::LeftButton) {
        ByteArrayTableCursor* const tableCursor = mView->tableCursor();
        ByteArrayTableRanges* const tableRanges = mView->tableRanges();
        const ByteArrayTableLayout* const tableLayout = mView->tableLayout();

        mView->pauseCursor();
        mView->finishByteEditor();

        mLMBPressed = true;

        // select whole line?
        if (mTrippleClickTimer.isActive()
            && (mouseEvent->globalPos() - mDoubleClickPoint).manhattanLength() < QApplication::startDragDistance()) {
            mTrippleClickTimer.stop();
            const Address indexAtFirstDoubleClickLinePosition = tableLayout->indexAtFirstLinePosition(mDoubleClickLine);
            tableRanges->setSelectionStart(indexAtFirstDoubleClickLinePosition);
            tableCursor->gotoIndex(indexAtFirstDoubleClickLinePosition);
            tableCursor->gotoLineEnd();
            tableRanges->setSelectionEnd(mView->cursorPosition());

            mView->updateChanged();
        } else {
            // TODO: pos() is now, not at the moment of the event, use globalPos() for that,.says dox
            const QPoint mousePoint = mView->viewportToColumns(mouseEvent->pos());

            // start of a drag perhaps?
            if (tableRanges->hasSelection() && tableRanges->selectionIncludes(mView->indexByPoint(mousePoint))) {
                mDragStartPossible = true;
                mDragStartTimer.start(QApplication::startDragTime());
                mDragStartPoint = mousePoint;
            } else {
                mView->placeCursor(mousePoint);
                mView->ensureCursorVisible();

                const Address realIndex = tableCursor->realIndex();
                if (tableRanges->selectionStarted()) {
                    if (mouseEvent->modifiers() & Qt::SHIFT) {
                        tableRanges->setSelectionEnd(realIndex);
                    } else {
                        tableRanges->removeSelection();
                        tableRanges->setSelectionStart(realIndex);
                    }
                } else {   // start of a new selection possible
                    tableRanges->setSelectionStart(realIndex);

                    if (!mView->isEffectiveReadOnly() && (mouseEvent->modifiers() & Qt::SHIFT)) { // TODO: why only for readwrite?
                        tableRanges->setSelectionEnd(realIndex);
                    }
                }

                tableRanges->removeFurtherSelections();
            }

            if (tableRanges->isModified()) {
                mView->updateChanged();
                mView->setMouseCursor(mView->isEffectiveReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor);
            }
        }

        mView->unpauseCursor();
        mView->emitSelectionUpdates();

        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMousePressEvent(mouseEvent);
}

bool MouseNavigator::handleMouseMoveEvent(QMouseEvent* mouseEvent)
{
    bool eventUsed = false;

    if (mouseEvent->buttons() == Qt::LeftButton) {
        const QPoint movePoint = mView->viewportToColumns(mouseEvent->pos());

        if (mLMBPressed) {
            if (mDragStartPossible) {
                mDragStartTimer.stop();
                // moved enough for a drag?
                if ((movePoint - mDragStartPoint).manhattanLength() > QApplication::startDragDistance()) {
                    startDrag();
                }
                if (!mView->isEffectiveReadOnly()) {
                    mView->setMouseCursor(Qt::IBeamCursor);
                }
            } else {
                // selecting
                handleMouseMove(movePoint);
            }
        } else if (!mView->isEffectiveReadOnly()) {
            const ByteArrayTableRanges* const tableRanges = mView->tableRanges();

            // visual feedback for possible dragging
            const bool InSelection =
                tableRanges->hasSelection() && tableRanges->selectionIncludes(mView->indexByPoint(movePoint));
            mView->setMouseCursor(InSelection ? Qt::ArrowCursor : Qt::IBeamCursor);
        }
        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMouseMoveEvent(mouseEvent);
}

bool MouseNavigator::handleMouseReleaseEvent(QMouseEvent* mouseEvent)
{
    bool eventUsed = false;

    if (mouseEvent->button() == Qt::LeftButton) {
        ByteArrayTableRanges* const tableRanges = mView->tableRanges();

//         const QPoint releasePoint = mView->viewportToColumns( mouseEvent->pos() );

        // this is not the release of a doubleclick so we need to process it?
        if (!mInLMBDoubleClick) {
//             const int line = mView->lineAt( releasePoint.y() );
//             const int pos = mActiveColumn->linePositionOfX( releasePoint.x() ); // TODO: can we be sure here about the active column?
//             const Address index = tableLayout->indexAtCCoord( Coord(pos,line) ); // TODO: can this be another index than the one of the cursor???
//             Q_EMIT mView->clicked( index ); // TODO: who needs this?
        }

        if (mLMBPressed) {
            mLMBPressed = false;

            if (mScrollTimer.isActive()) {
                mScrollTimer.stop();
            }

            // was only click inside selection, nothing dragged?
            if (mDragStartPossible) {
                mView->selectAll(false);
                mDragStartTimer.stop();
                mDragStartPossible = false;

                mView->placeCursor(mDragStartPoint);
                mView->ensureCursorVisible();

                mView->unpauseCursor();
            }
            // was end of selection operation?
            else if (tableRanges->hasSelection()) {
                if (QApplication::clipboard()->supportsSelection()) {
                    mView->copyToClipboard(QClipboard::Selection);
                }
            }
        }

        mView->emitCursorPositionChanged();

        mInLMBDoubleClick = false;

        if (tableRanges->selectionJustStarted()) {
            tableRanges->removeSelection();
        }

        mView->emitSelectionUpdates();
        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMouseReleaseEvent(mouseEvent);
}

bool MouseNavigator::handleMouseDoubleClickEvent(QMouseEvent* mouseEvent)
{
    bool eventUsed = false;

    if (mouseEvent->button() == Qt::LeftButton) {
        const ByteArrayTableCursor* const tableCursor = mView->tableCursor();

        mDoubleClickLine = tableCursor->line();

        const Address index = tableCursor->validIndex();

        if (mView->activeCoding() == AbstractByteArrayView::CharCodingId) {
            std::ignore = mView->selectWord(index);

            // as we already have a doubleclick maybe it is a tripple click
            mTrippleClickTimer.start(qApp->doubleClickInterval());
            mDoubleClickPoint = mouseEvent->globalPos();
        }
        //  else
        //    mValueEditor->goInsideByte(); TODO: make this possible again

        mInLMBDoubleClick = true; //
        mLMBPressed = true;

        mView->emitDoubleClicked(index);
        eventUsed = true;
    }

    return eventUsed ? true : AbstractMouseController::handleMouseDoubleClickEvent(mouseEvent);
}

void MouseNavigator::autoScrollTimerDone()
{
    if (mLMBPressed) {
        handleMouseMove(mView->viewportToColumns(mView->mapViewportFromGlobal(QCursor::pos())));
    }
}

void MouseNavigator::handleMouseMove(QPoint point)   // handles the move of the mouse with pressed buttons
{
    ByteArrayTableCursor* const tableCursor = mView->tableCursor();
    ByteArrayTableRanges* const tableRanges = mView->tableRanges();

    const int yOffset = mView->yOffset();
    const int behindLastYOffset = yOffset + mView->visibleHeight();
    // scrolltimer but inside of viewport?
    if (mScrollTimer.isActive()) {
        if (yOffset <= point.y() && point.y() < behindLastYOffset) {
            mScrollTimer.stop();
        }
    }
    // no scrolltimer and outside of viewport?
    else {
        if (point.y() < yOffset || behindLastYOffset <= point.y()) {
            mScrollTimer.start(DefaultScrollTimerPeriod);
        }
    }
    mView->pauseCursor();

    mView->placeCursor(point);
    mView->ensureCursorVisible();

    // do wordwise selection?
    if (mInLMBDoubleClick && tableRanges->hasFirstWordSelection()) {
        Address newIndex = tableCursor->realIndex();
        const AddressRange firstWordSelection = tableRanges->firstWordSelection();
        const TextByteArrayAnalyzer textAnalyzer(mView->byteArrayModel(), mView->charCodec());
        // are we before the selection?
        if (firstWordSelection.startsBehind(newIndex)) {
            tableRanges->ensureWordSelectionForward(false);
            newIndex = textAnalyzer.indexOfLeftWordSelect(newIndex);
        }
        // or behind?
        else if (firstWordSelection.endsBefore(newIndex)) {
            tableRanges->ensureWordSelectionForward(true);
            newIndex = textAnalyzer.indexOfRightWordSelect(newIndex);
        }
        // or inside?
        else {
            tableRanges->ensureWordSelectionForward(true);
            newIndex = firstWordSelection.nextBehindEnd();
        }

        tableCursor->gotoIndex(newIndex);
    }

    if (tableRanges->selectionStarted()) {
        tableRanges->setSelectionEnd(mView->cursorPosition());
    }

    mView->updateChanged();
    mView->unpauseCursor();
    mView->emitSelectionUpdates();
}

void MouseNavigator::startDrag()
{
    // reset states
    mLMBPressed = false;
    mInLMBDoubleClick = false;
    mDragStartPossible = false;

    // create data
    auto dragData = mView->selectedBytesAsMimeData();
    if (!dragData) {
        return;
    }

    // QDrag instance owned by Qt, gets deleted internally by deleteLater after exec() call
    auto* drag = new QDrag(mView->q_func());
    drag->setMimeData(dragData.release());

    Qt::DropActions request = (mView->isEffectiveReadOnly() || mView->isOverwriteMode()) ? Qt::CopyAction : Qt::CopyAction | Qt::MoveAction;
    Qt::DropAction dropAction = drag->exec(request);

    if (dropAction == Qt::MoveAction) {
        auto* targetByteArrayView = qobject_cast<AbstractByteArrayView*>(drag->target());
        // Not inside this widget itself?
        if (!targetByteArrayView
            || targetByteArrayView->byteArrayModel() != mView->byteArrayModel()) {
            mView->removeSelectedBytes();
        }
    }
}

}

#include "moc_mousenavigator.cpp"
