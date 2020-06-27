/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycolumnview_p.hpp"

// lib
#include "oktetagui.hpp"
#include "bordercolumnrenderer.hpp"
#include "widgetcolumnstylist.hpp"
#include "controller/dropper.hpp"
#include "cursor.hpp"
// Okteta core
#include <Okteta/ValueCodec>
// Qt
#include <QStyle>
#include <QFontDatabase>
#include <QPainter>
#include <QScrollBar>
#include <QEvent>

namespace Okteta {

ByteArrayColumnViewPrivate::ByteArrayColumnViewPrivate(ByteArrayColumnView* parent)
    : AbstractByteArrayViewPrivate(parent)
{
}

ByteArrayColumnViewPrivate::~ByteArrayColumnViewPrivate() = default;

void ByteArrayColumnViewPrivate::init()
{
    Q_Q(ByteArrayColumnView);

    // creating the columns in the needed order
    mValueColumn =
        new ValueByteArrayColumnRenderer(mStylist, mByteArrayModel, mTableLayout, mTableRanges);
    mMiddleBorderColumn =
        new BorderColumnRenderer(mStylist, true);
    mCharColumn =
        new CharByteArrayColumnRenderer(mStylist, mByteArrayModel, mTableLayout, mTableRanges);

    q->addColumn(mOffsetColumn);
    q->addColumn(mOffsetBorderColumn);
    q->addColumn(mValueColumn);
    q->addColumn(mMiddleBorderColumn);
    q->addColumn(mCharColumn);

    // select the active column
    mActiveColumn = mCharColumn;
    mInactiveColumn = mValueColumn;

    // set char encoding
    mValueColumn->setValueCodec((ValueCoding)mValueCoding, mValueCodec);
    mValueColumn->setCharCodec(mCharCodec);
    mCharColumn->setCharCodec(mCharCodec);

    adaptController();

    // do here, not in base class, as changeEvent(fontEvent) needs this init run before
    q->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

AbstractByteArrayView::CodingTypeId ByteArrayColumnViewPrivate::activeCoding() const
{
    const bool isValueColumnActive = (mActiveColumn == (AbstractByteArrayColumnRenderer*)mValueColumn);
    return isValueColumnActive ? AbstractByteArrayView::ValueCodingId : AbstractByteArrayView::CharCodingId;
}

AbstractByteArrayView::CodingTypes ByteArrayColumnViewPrivate::visibleCodings() const
{
    // TODO: try to improve this code
    return (AbstractByteArrayView::CodingTypes)
           ((mValueColumn->isVisible() ? AbstractByteArrayView::ValueCodingId : 0)
            | (mCharColumn->isVisible() ? AbstractByteArrayView::CharCodingId : 0));
}

void ByteArrayColumnViewPrivate::setByteArrayModel(AbstractByteArrayModel* _byteArrayModel)
{
    mValueEditor->reset();

    // TODO: this fails if _byteArrayModel == null
    mValueColumn->set(_byteArrayModel);
    mCharColumn->set(_byteArrayModel);

    AbstractByteArrayViewPrivate::setByteArrayModel(_byteArrayModel);
}

void ByteArrayColumnViewPrivate::setBufferSpacing(PixelX ByteSpacing, int noOfGroupedBytes, PixelX GroupSpacing)
{
    if (!mValueColumn->setSpacing(ByteSpacing, noOfGroupedBytes, GroupSpacing)) {
        return;
    }

    updateViewByWidth();
}

void ByteArrayColumnViewPrivate::setValueCoding(AbstractByteArrayView::ValueCoding valueCoding)
{
    Q_Q(ByteArrayColumnView);

    if (mValueCoding == valueCoding) {
        return;
    }

    const uint oldCodingWidth = mValueCodec->encodingWidth();

    AbstractByteArrayViewPrivate::setValueCoding(valueCoding);

    mValueColumn->setValueCodec((ValueCoding)mValueCoding, mValueCodec);
    mValueEditor->adaptToValueCodecChange();

    const uint newCodingWidth = mValueCodec->encodingWidth();

    // no change in the width?
    if (newCodingWidth == oldCodingWidth) {
        q->updateColumn(*mValueColumn);
    } else {
        updateViewByWidth();
    }

    emit q->valueCodingChanged(valueCoding);
}

void ByteArrayColumnViewPrivate::setByteSpacingWidth(int /*PixelX*/ byteSpacingWidth)
{
    if (!mValueColumn->setByteSpacingWidth(byteSpacingWidth)) {
        return;
    }
    updateViewByWidth();
}

void ByteArrayColumnViewPrivate::setNoOfGroupedBytes(int noOfGroupedBytes)
{
    Q_Q(ByteArrayColumnView);

    if (!mValueColumn->setNoOfGroupedBytes(noOfGroupedBytes)) {
        return;
    }
    updateViewByWidth();

    emit q->noOfGroupedBytesChanged(noOfGroupedBytes);
}

void ByteArrayColumnViewPrivate::setGroupSpacingWidth(int /*PixelX*/ groupSpacingWidth)
{
    if (!mValueColumn->setGroupSpacingWidth(groupSpacingWidth)) {
        return;
    }
    updateViewByWidth();
}

void ByteArrayColumnViewPrivate::setBinaryGapWidth(int /*PixelX*/ binaryGapWidth)
{
    if (!mValueColumn->setBinaryGapWidth(binaryGapWidth)) {
        return;
    }
    updateViewByWidth();
}

void ByteArrayColumnViewPrivate::setSubstituteChar(QChar substituteChar)
{
    Q_Q(ByteArrayColumnView);

    if (!mCharColumn->setSubstituteChar(substituteChar)) {
        return;
    }
    pauseCursor();
    q->updateColumn(*mCharColumn);
    unpauseCursor();

    emit q->substituteCharChanged(substituteChar);
}

void ByteArrayColumnViewPrivate::setUndefinedChar(QChar undefinedChar)
{
    Q_Q(ByteArrayColumnView);

    if (!mCharColumn->setUndefinedChar(undefinedChar)) {
        return;
    }
    pauseCursor();
    q->updateColumn(*mCharColumn);
    unpauseCursor();

    emit q->undefinedCharChanged(undefinedChar);
}

void ByteArrayColumnViewPrivate::setShowsNonprinting(bool showingNonprinting)
{
    Q_Q(ByteArrayColumnView);

    if (!mCharColumn->setShowingNonprinting(showingNonprinting)) {
        return;
    }
    pauseCursor();
    q->updateColumn(*mCharColumn);
    unpauseCursor();

    emit q->showsNonprintingChanged(showingNonprinting);
}

void ByteArrayColumnViewPrivate::setCharCoding(AbstractByteArrayView::CharCoding charCoding)
{
    Q_Q(ByteArrayColumnView);

    if (mCharCoding == charCoding) {
        return;
    }

    AbstractByteArrayViewPrivate::setCharCoding(charCoding);

    pauseCursor();

    mValueColumn->setCharCodec(mCharCodec);
    mCharColumn->setCharCodec(mCharCodec);

    q->updateColumn(*mValueColumn);
    q->updateColumn(*mCharColumn);

    unpauseCursor();

    emit q->charCodecChanged(charCodingName());
}

// TODO: join with function above!
void ByteArrayColumnViewPrivate::setCharCoding(const QString& newCharCodingName)
{
    Q_Q(ByteArrayColumnView);

    if (charCodingName() == newCharCodingName) {
        return;
    }

    AbstractByteArrayViewPrivate::setCharCoding(newCharCodingName);

    pauseCursor();

    mValueColumn->setCharCodec(mCharCodec);
    mCharColumn->setCharCodec(mCharCodec);

    q->updateColumn(*mValueColumn);
    q->updateColumn(*mCharColumn);

    unpauseCursor();

    emit q->charCodecChanged(charCodingName());
}

void ByteArrayColumnViewPrivate::setByteTypeColored(bool isColored)
{
    Q_Q(ByteArrayColumnView);

    if (isColored == mValueColumn->isByteTypeColored()) {
        return;
    }

    mValueColumn->setByteTypeColored(isColored);
    mCharColumn->setByteTypeColored(isColored);

    pauseCursor();
    q->updateColumn(*mValueColumn);
    q->updateColumn(*mCharColumn);
    unpauseCursor();
}

void ByteArrayColumnViewPrivate::changeEvent(QEvent* event)
{
    Q_Q(ByteArrayColumnView);

    // make sure base class deals with this event first,
    // as some values which are used here are updated there
    q->AbstractByteArrayView::changeEvent(event);

    if (event->type() != QEvent::FontChange) {
        return;
    }

    // get new values
    const QFontMetrics newFontMetrics = q->fontMetrics();
    const PixelY digitHeight = newFontMetrics.height();

    mOffsetColumn->setFontMetrics(newFontMetrics);
    mValueColumn->setFontMetrics(newFontMetrics);
    mCharColumn->setFontMetrics(newFontMetrics);

    q->setLineHeight(digitHeight);

    // update all dependent structures
    mTableLayout->setNoOfLinesPerPage(q->noOfLinesPerPage());

    updateViewByWidth();
}

void ByteArrayColumnViewPrivate::adjustToLayoutNoOfBytesPerLine()
{
    Q_Q(ByteArrayColumnView);

    mValueColumn->resetXBuffer();
    mCharColumn->resetXBuffer();

    q->updateWidths();
}

QSize ByteArrayColumnViewPrivate::minimumSizeHint() const
{
    Q_Q(const ByteArrayColumnView);

    // TODO: better minimal width (visibility!)
    const int minWidth =
        mOffsetColumn->visibleWidth()
        + mOffsetBorderColumn->visibleWidth()
        + mMiddleBorderColumn->visibleWidth()
        + mValueColumn->byteWidth()
        + mCharColumn->byteWidth();
    const int minHeight =
        q->lineHeight()
        + q->noOfLines() > 1 ? q->style()->pixelMetric(QStyle::PM_ScrollBarExtent) : 0;

    return {qMin(minWidth, 100), qMin(minHeight, 100)};
}

int ByteArrayColumnViewPrivate::fittingBytesPerLine() const
{
    Q_Q(const ByteArrayColumnView);

    const QSize newSize = q->maximumViewportSize();
    const PixelX reservedWidth =
        mOffsetColumn->visibleWidth()
        + mOffsetBorderColumn->visibleWidth()
        + mMiddleBorderColumn->visibleWidth();

    // abstract offset and border columns width
    const PixelX fullWidth = newSize.width() - reservedWidth;

    //  // no width left for resizeable columns? TODO: put this in resizeEvent
    //  if( fullWidth < 0 )
    //    return;

    const PixelY fullHeight = newSize.height();

    // check influence of dis-/appearing of the vertical scrollbar
    const bool verticalScrollbarIsVisible = q->verticalScrollBar()->isVisible();
    const PixelX scrollbarExtent = q->style()->pixelMetric(QStyle::PM_ScrollBarExtent);

    PixelX availableWidth = fullWidth;
    if (verticalScrollbarIsVisible) {
        availableWidth -= scrollbarExtent;
    }

    enum MatchTrial
    {
        FirstRun,
        RerunWithScrollbarOn,
        TestWithoutScrollbar
    };
    MatchTrial matchRun = FirstRun;

    // prepare needed values
    const PixelX digitWidth = mValueColumn->digitWidth();
    const PixelX charByteWidth = mCharColumn->isVisible() ? digitWidth : 0;
    const PixelX valueByteWidth = mValueColumn->isVisible() ? mValueColumn->byteWidth() : 0;
    const PixelX byteSpacingWidth = mValueColumn->isVisible() ? mValueColumn->byteSpacingWidth() : 0;
    PixelX groupSpacingWidth;
    int noOfGroupedBytes = mValueColumn->noOfGroupedBytes();
    // no grouping?
    if (noOfGroupedBytes == 0) {
        // fake no grouping by grouping with 1 and using byteSpacingWidth
        noOfGroupedBytes = 1;
        groupSpacingWidth = mValueColumn->isVisible() ? byteSpacingWidth : 0;
    } else {
        groupSpacingWidth = mValueColumn->isVisible() ? mValueColumn->groupSpacingWidth() : 0;
    }

    const PixelX valueByteGroupWidth =  noOfGroupedBytes * valueByteWidth + (noOfGroupedBytes - 1) * byteSpacingWidth;
    const PixelX charByteGroupWidth = noOfGroupedBytes * charByteWidth;
    const PixelX totalGroupWidth = valueByteGroupWidth + groupSpacingWidth + charByteGroupWidth;

    int fittingBytesPerLine;
    int fittingBytesPerLineWithScrollbar = 0;
    for (;;) {
        //    qCDebug(LOG_OKTETA_GUI) << "matchWidth: " << fullWidth
        //              << " (v:" << visibleWidth()
        //              << ", f:" << frameWidth()
        //              << ", A:" << availableWidth
        //              << ", S:" << scrollbarExtent
        //              << ", R:" << reservedWidth << ")" << endl;

        // calculate fitting groups per line
        const int fittingGroupsPerLine = (availableWidth + groupSpacingWidth) // fake spacing after last group
                                         / totalGroupWidth;

        // calculate the fitting bytes per line by groups
        fittingBytesPerLine = noOfGroupedBytes * fittingGroupsPerLine;

        // not only full groups?
        if (mResizeStyle == AbstractByteArrayView::FullSizeLayoutStyle && noOfGroupedBytes > 1) {

            if (fittingGroupsPerLine > 0) {
                availableWidth -= fittingGroupsPerLine * totalGroupWidth; // includes additional spacing after last group

            }
//         qCDebug(LOG_OKTETA_GUI) << "Left: " << availableWidth << "("<<valueByteWidth<<", "<<charByteWidth<<")" ;

            if (availableWidth > 0) {
                fittingBytesPerLine += (availableWidth + byteSpacingWidth) / (valueByteWidth + byteSpacingWidth + charByteWidth);
            }

            // is there not even the space for a single byte?
            if (fittingBytesPerLine == 0) {
                // ensure at least one byte per line
                fittingBytesPerLine = 1;
                // and
                break;
            }
        }
        // is there not the space for a single group?
        else if (fittingBytesPerLine == 0) {
            // ensures at least one group
            fittingBytesPerLine = noOfGroupedBytes;
            break;
        }

//    qCDebug(LOG_OKTETA_GUI) << "meantime: " << fittingGroupsPerLine << " (T:" << totalGroupWidth
//              << ", h:" << valueByteGroupWidth
//              << ", t:" << charByteGroupWidth
//              << ", s:" << groupSpacingWidth << ") " <<fittingBytesPerLine<< endl;

        const int newNoOfLines = (mTableLayout->length() + mTableLayout->startOffset() + fittingBytesPerLine - 1)
                                 / fittingBytesPerLine;
        const PixelY newHeight =  newNoOfLines * q->lineHeight();

        if (verticalScrollbarIsVisible) {
            if (matchRun == TestWithoutScrollbar) {
                // did the test without the scrollbar fail, don't the data fit into the view?
                if (newHeight > fullHeight) {
                    // reset to old calculated value
                    fittingBytesPerLine =  fittingBytesPerLineWithScrollbar;
                }
                break;
            }

            // a chance for to perhaps fit in height?
            if (fittingBytesPerLine <= mTableLayout->noOfBytesPerLine()) {
                // remember this trial's result and calc number of bytes with vertical scrollbar on
                fittingBytesPerLineWithScrollbar = fittingBytesPerLine;
                availableWidth = fullWidth;
                matchRun = TestWithoutScrollbar;
                //          qCDebug(LOG_OKTETA_GUI) << "tested without scrollbar..." ;
                continue;
            }
        } else {
            // doesn't it fit into the height anymore?
            if (newHeight > fullHeight && matchRun == FirstRun) {
                // need for a scrollbar has risen... ->less width, new calculation
                availableWidth = fullWidth - scrollbarExtent;
                matchRun = RerunWithScrollbarOn;
                //          qCDebug(LOG_OKTETA_GUI) << "rerun with scrollbar on..." ;
                continue;
            }
        }

        break;
    }

    return fittingBytesPerLine;
}

void ByteArrayColumnViewPrivate::setVisibleCodings(int newColumns)
{
    Q_Q(ByteArrayColumnView);

    const int oldColumns = visibleCodings();

    // no changes or no column selected?
    if (newColumns == oldColumns || !(newColumns & AbstractByteArrayView::ValueAndCharCodings)) {
        return;
    }

    mValueColumn->setVisible(AbstractByteArrayView::ValueCodingId & newColumns);
    mCharColumn->setVisible(AbstractByteArrayView::CharCodingId & newColumns);
    mMiddleBorderColumn->setVisible(newColumns == AbstractByteArrayView::ValueAndCharCodings);

    // active column not visible anymore?
    if (!mActiveColumn->isVisible()) {
        AbstractByteArrayColumnRenderer* h = mActiveColumn;
        mActiveColumn = mInactiveColumn;
        mInactiveColumn = h;
        adaptController();
    }

    updateViewByWidth();

    emit q->visibleByteArrayCodingsChanged(newColumns);
}

void ByteArrayColumnViewPrivate::setActiveCoding(AbstractByteArrayView::CodingTypeId codingId)
{
    // no changes or not visible?
    if (codingId == activeCoding()
        || (codingId == AbstractByteArrayView::ValueCodingId && !mValueColumn->isVisible())
        || (codingId == AbstractByteArrayView::CharCodingId && !mCharColumn->isVisible())) {
        return;
    }

    pauseCursor();
    mValueEditor->finishEdit();

    if (codingId == AbstractByteArrayView::ValueCodingId) {
        mActiveColumn = mValueColumn;
        mInactiveColumn = mCharColumn;
    } else {
        mActiveColumn = mCharColumn;
        mInactiveColumn = mValueColumn;
    }
    adaptController();

    ensureCursorVisible();
    unpauseCursor();
}

void ByteArrayColumnViewPrivate::placeCursor(QPoint point)
{
    Q_Q(ByteArrayColumnView);

    // switch active column if needed
    if (mCharColumn->isVisible() && point.x() >= mCharColumn->x()) {
        mActiveColumn = mCharColumn;
        mInactiveColumn = mValueColumn;
    } else {
        mActiveColumn = mValueColumn;
        mInactiveColumn = mCharColumn;
    }
    adaptController();

    // get coord of click and whether this click was closer to the end of the pos
    const int linePosition = mActiveColumn->magneticLinePositionOfX(point.x());
    const int lineIndex = q->lineAt(point.y());
    const Coord coord(linePosition, lineIndex);

    mTableCursor->gotoCCoord(coord);
    emit q->cursorPositionChanged(cursorPosition());
}

Address ByteArrayColumnViewPrivate::indexByPoint(QPoint point) const
{
    Q_Q(const ByteArrayColumnView);

    const AbstractByteArrayColumnRenderer* column =
        (mCharColumn->isVisible() && point.x() >= mCharColumn->x()) ?
        (AbstractByteArrayColumnRenderer*)mCharColumn : (AbstractByteArrayColumnRenderer*)mValueColumn;

    const Coord coord(column->linePositionOfX(point.x()), q->lineAt(point.y()));

    return mTableLayout->indexAtCCoord(coord);
}

void ByteArrayColumnViewPrivate::blinkCursor()
{
    // skip the cursor drawing?
    if (mCursorPaused || mValueEditor->isInEditMode()) {
        return;
    }

    // switch the cursor state
    mBlinkCursorVisible = !mBlinkCursorVisible;
    updateCursor(*mActiveColumn);
}

void ByteArrayColumnViewPrivate::updateCursors()
{
    createCursorPixmaps();

    mBlinkCursorVisible = true;
    updateCursor(*mActiveColumn);
    updateCursor(*mInactiveColumn);
}

void ByteArrayColumnViewPrivate::pauseCursor()
{
    mCursorPaused = true;

    mBlinkCursorVisible = false;
    updateCursor(*mActiveColumn);
    updateCursor(*mInactiveColumn);
}

QRect ByteArrayColumnViewPrivate::cursorRect() const
{
    Q_Q(const ByteArrayColumnView);

    QRect cursorRect = mActiveColumn->byteRect(mTableCursor->coord());
    cursorRect.translate(-q->xOffset(), -q->yOffset());

    return cursorRect;
}

void ByteArrayColumnViewPrivate::updateCursor(const AbstractByteArrayColumnRenderer& column)
{
    Q_Q(ByteArrayColumnView);

    if (!column.isVisible()) {
        return;
    }

    QRect cursorRect = column.byteRect(mTableCursor->coord());
    cursorRect.translate(-q->xOffset(), -q->yOffset());

    q->viewport()->update(cursorRect);
}

void ByteArrayColumnViewPrivate::createCursorPixmaps()
{
    Q_Q(ByteArrayColumnView);

    const PixelX byteWidth = mActiveColumn->byteWidth();

    // create mCursorPixmaps
    mCursorPixmaps->setSize(byteWidth, q->lineHeight(), q->devicePixelRatio());

    const Address index = mTableCursor->validIndex();

    QPainter painter;
    painter.begin(&mCursorPixmaps->offPixmap());
    initPainterFromWidget(&painter);
    mActiveColumn->renderByte(&painter, index);
    painter.end();

    painter.begin(&mCursorPixmaps->onPixmap());
    initPainterFromWidget(&painter);
    mActiveColumn->renderCursor(&painter, index);
    painter.end();

    // calculat the shape
    PixelX cursorX;
    PixelX cursorW;
    if (isCursorBehind()) {
        cursorX = qMax(0, byteWidth - InsertCursorWidth);
        cursorW = InsertCursorWidth;
    } else {
        cursorX = 0;
        cursorW = mOverWrite ? -1 : InsertCursorWidth;
    }
    mCursorPixmaps->setShape(cursorX, cursorW, q->devicePixelRatio());
}

void ByteArrayColumnViewPrivate::drawActiveCursor(QPainter* painter)
{
    Q_Q(ByteArrayColumnView);

    // TODO: review the cursor drawing, not fully matching the new paint only on updates yet
    // see also rowView

    // any reason to skip the cursor drawing?
    if (!q->hasFocus() && !q->viewport()->hasFocus() && !mDropper->isActive()) {
        return;
    }

    const int x = mActiveColumn->xOfLinePosition(mTableCursor->pos());
    const int y = q->lineHeight() * mTableCursor->line();

    painter->translate(x, y);

    // paint edited byte?
    if (mValueEditor->isInEditMode()) {
        const Address index = mTableCursor->index();

        if (mBlinkCursorVisible) {
            mValueColumn->renderEditedByte(painter, mValueEditor->value(), mValueEditor->valueAsString());
        } else {
            mValueColumn->renderByte(painter, index);
        }
    } else {
        painter->drawPixmap(mCursorPixmaps->cursorX(), 0,
                            mBlinkCursorVisible ? mCursorPixmaps->onPixmap() : mCursorPixmaps->offPixmap(),
                            mCursorPixmaps->shapeX(), 0, mCursorPixmaps->shapeW(), -1);
    }

    painter->translate(-x, -y);
}

void ByteArrayColumnViewPrivate::drawInactiveCursor(QPainter* painter)
{
    Q_Q(ByteArrayColumnView);

    // any reason to skip the cursor drawing?
    if (!mInactiveColumn->isVisible()
        || mCursorPaused
        || (!mCursorPaused && !q->hasFocus() && !q->viewport()->hasFocus() && !mDropper->isActive())) {
        return;
    }

    const Address index = mTableCursor->validIndex();

    const int x = mInactiveColumn->xOfLinePosition(mTableCursor->pos());
    const int y = q->lineHeight() * mTableCursor->line();

    painter->translate(x, y);

    const AbstractByteArrayColumnRenderer::FrameStyle frameStyle =
        mTableCursor->isBehind() ?                    AbstractByteArrayColumnRenderer::Right :
        (mOverWrite || mValueEditor->isInEditMode()) ? AbstractByteArrayColumnRenderer::Frame :
        AbstractByteArrayColumnRenderer::Left;
    mInactiveColumn->renderFramedByte(painter, index, frameStyle);

    painter->translate(-x, -y);
}

void ByteArrayColumnViewPrivate::renderColumns(QPainter* painter, int cx, int cy, int cw, int ch)
{
    Q_Q(ByteArrayColumnView);

    q->AbstractByteArrayView::renderColumns(painter, cx, cy, cw, ch);
    // TODO: update non blinking cursors. Should this perhaps be done in the buffercolumn?
    // Then it needs to know about inactive, insideByte and the like... well...
    // perhaps subclassing the buffer columns even more, to CharByteArrayColumnRenderer and ValueByteArrayColumnRenderer?

    if (q->visibleLines(PixelYRange::fromWidth(cy, ch)).includes(mTableCursor->line())) {
        drawActiveCursor(painter);
        drawInactiveCursor(painter);
    }
}

void ByteArrayColumnViewPrivate::updateChanged()
{
    Q_Q(ByteArrayColumnView);

    const int xOffset = q->xOffset();
    const PixelXRange Xs = PixelXRange::fromWidth(xOffset, q->visibleWidth());

    // do updates in offset column
    const LineRange changedOffsetLines = mTableRanges->changedOffsetLines();
    if (!changedOffsetLines.isEmpty()) {
        q->updateColumn(*mOffsetColumn, changedOffsetLines);
    }

    // collect affected buffer columns
    QVector<AbstractByteArrayColumnRenderer*> dirtyColumns;
    dirtyColumns.reserve(2);

    AbstractByteArrayColumnRenderer* column = mValueColumn;
    while (true) {
        if (column->isVisible() && column->overlaps(Xs)) {
            dirtyColumns.append(column);
            column->prepareRendering(Xs);
        }

        if (column == mCharColumn) {
            break;
        }
        column = mCharColumn;
    }

    // any columns to paint?
    if (!dirtyColumns.isEmpty()) {
        // calculate affected lines/indizes
        const LinePositionRange fullPositions(0, mTableLayout->noOfBytesPerLine() - 1);
        CoordRange visibleRange(fullPositions, q->visibleLines());

        const int lineHeight = q->lineHeight();
        CoordRange changedRange;
        // as there might be multiple selections on this line redo until no more is changed
        while (getNextChangedRange(&changedRange, visibleRange)) {
            PixelY cy = q->yOffsetOfLine(changedRange.start().line());

            // only one line?
            if (changedRange.start().line() == changedRange.end().line()) {
                const LinePositionRange changedPositions(changedRange.start().pos(), changedRange.end().pos());
                for (auto column : qAsConst(dirtyColumns)) {
                    const PixelXRange xPixels = column->xsOfLinePositionsInclSpaces(changedPositions);

                    q->viewport()->update(xPixels.start() - xOffset, cy, xPixels.width(), lineHeight);
                }
            }
            //
            else {
                // first line
                const LinePositionRange firstChangedPositions(changedRange.start().pos(), fullPositions.end());
                for (auto column : qAsConst(dirtyColumns)) {
                    const PixelXRange XPixels = column->xsOfLinePositionsInclSpaces(firstChangedPositions);

                    q->viewport()->update(XPixels.start() - xOffset, cy, XPixels.width(), lineHeight);
                }

                // at least one full line?
                for (int l = changedRange.start().line() + 1; l < changedRange.end().line(); ++l) {
                    cy += lineHeight;
                    for (auto column : qAsConst(dirtyColumns)) {
                        const PixelXRange XPixels = column->xsOfLinePositionsInclSpaces(fullPositions);

                        q->viewport()->update(XPixels.start() - xOffset, cy, XPixels.width(), lineHeight);
                    }
                }

                // last line
                cy += lineHeight;
                const LinePositionRange lastChangedPositions(fullPositions.start(), changedRange.end().pos());
                for (auto column : qAsConst(dirtyColumns)) {
                    const PixelXRange XPixels = column->xsOfLinePositionsInclSpaces(lastChangedPositions);

                    q->viewport()->update(XPixels.start() - xOffset, cy, XPixels.width(), lineHeight);
                }
            }

            // continue the search at the overnext index
            visibleRange.setStart(changedRange.end() + 1); // +2 ); TODO: currently bounding ranges are not merged
            if (!visibleRange.isValid()) {
                break;
            }
        }
    }

    mTableRanges->resetChangedRanges();
}

void ByteArrayColumnViewPrivate::ensureCursorVisible()
{
    ensureVisible(*mActiveColumn, mTableCursor->coord());
}

void ByteArrayColumnViewPrivate::ensureVisible(const AddressRange& range, bool ensureStartVisible)
{
    const CoordRange coords = mTableLayout->coordRangeOfIndizes(range);

    // TODO: this is a make-it-work-hack, better do a smart calculation
    ensureVisible(*mActiveColumn, ensureStartVisible ? coords.end() : coords.start());
    ensureVisible(*mActiveColumn, ensureStartVisible ? coords.start() : coords.end());
}

void ByteArrayColumnViewPrivate::ensureVisible(const AbstractByteArrayColumnRenderer& column, Coord coord)
{
    Q_Q(ByteArrayColumnView);

    const QRect byteRect = column.byteRect(coord);

    const PixelXRange byteXs = PixelXRange::fromWidth(byteRect.x(), byteRect.width());
    const PixelYRange byteYs = PixelYRange::fromWidth(byteRect.y(), byteRect.height());

    const PixelXRange visibleXs = PixelXRange::fromWidth(q->xOffset(), q->visibleWidth());
    const PixelYRange visibleYs = PixelXRange::fromWidth(q->yOffset(), q->visibleHeight());

    q->horizontalScrollBar()->setValue(visibleXs.startForInclude(byteXs));
    q->verticalScrollBar()->setValue(visibleYs.startForInclude(byteYs));
}

}
