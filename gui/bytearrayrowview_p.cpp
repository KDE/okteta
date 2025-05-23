/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrowview_p.hpp"

// lib
#include "oktetagui.hpp"
#include "bordercolumnrenderer.hpp"
#include "widgetcolumnstylist.hpp"
#include "controller/dropper.hpp"
#include <logging.hpp>
// Okteta core
#include <Okteta/ValueCodec>
// Qt
#include <QStyle>
#include <QFontDatabase>
#include <QPainter>
#include <QScrollBar>
// Std
#include <utility>
#include <vector>

namespace Okteta {

ByteArrayRowViewPrivate::ByteArrayRowViewPrivate(ByteArrayRowView* parent)
    : AbstractByteArrayViewPrivate(parent)
{
}

ByteArrayRowViewPrivate::~ByteArrayRowViewPrivate() = default;

void ByteArrayRowViewPrivate::init()
{
    Q_Q(ByteArrayRowView);

    // creating the columns in the needed order
    auto byteArrayColumn =
        std::make_unique<ByteArrayRowColumnRenderer>(mStylist.get(), mByteArrayModel, &mTableLayout, &mTableRanges);
    mByteArrayColumn = byteArrayColumn.get();
    addColumn(std::move(byteArrayColumn));

    // select the active column
    mActiveCoding = AbstractByteArrayView::CharCodingId;
    mInactiveCoding = AbstractByteArrayView::ValueCodingId;

    // set char encoding
    mByteArrayColumn->setValueCodec((ValueCoding)mValueCoding, mValueCodec.get());
    mByteArrayColumn->setCharCodec(mCharCodec.get());
//     mByteArrayColumn->setActiveCoding( mActiveCoding );

    adaptController();

    // do here, not in base class, as changeEvent(fontEvent) needs this init run before
    q->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

AbstractByteArrayView::CodingTypes ByteArrayRowViewPrivate::visibleCodings() const { return mByteArrayColumn->visibleCodings(); }
ByteArrayRowView::CodingTypeId ByteArrayRowViewPrivate::activeCoding() const { return mActiveCoding; }

void ByteArrayRowViewPrivate::setByteArrayModel(AbstractByteArrayModel* _byteArrayModel)
{
    mValueEditor.reset();

    // TODO: this fails if _byteArrayModel == null
    mByteArrayColumn->set(_byteArrayModel);

    AbstractByteArrayViewPrivate::setByteArrayModel(_byteArrayModel);
}

void ByteArrayRowViewPrivate::setBufferSpacing(PixelX ByteSpacing, int noOfGroupedBytes, PixelX GroupSpacing)
{
    if (!mByteArrayColumn->setSpacing(ByteSpacing, noOfGroupedBytes, GroupSpacing)) {
        return;
    }

    updateViewByWidth();
}

void ByteArrayRowViewPrivate::setValueCoding(AbstractByteArrayView::ValueCoding valueCoding)
{
    Q_Q(ByteArrayRowView);

    if (mValueCoding == valueCoding) {
        return;
    }

    const uint oldCodingWidth = mValueCodec->encodingWidth();

    AbstractByteArrayViewPrivate::setValueCoding(valueCoding);

    mByteArrayColumn->setValueCodec((ValueCoding)mValueCoding, mValueCodec.get());
    mValueEditor.adaptToValueCodecChange();

    const uint newCodingWidth = mValueCodec->encodingWidth();

    // no change in the width?
    if (newCodingWidth == oldCodingWidth) {
        updateColumn(*mByteArrayColumn);
    } else {
        updateViewByWidth();
    }

    Q_EMIT q->valueCodingChanged(valueCoding);
}

void ByteArrayRowViewPrivate::setByteSpacingWidth(int /*PixelX*/ byteSpacingWidth)
{
    if (!mByteArrayColumn->setByteSpacingWidth(byteSpacingWidth)) {
        return;
    }
    updateViewByWidth();
}

void ByteArrayRowViewPrivate::setNoOfGroupedBytes(int noOfGroupedBytes)
{
    Q_Q(ByteArrayRowView);

    if (!mByteArrayColumn->setNoOfGroupedBytes(noOfGroupedBytes)) {
        return;
    }
    updateViewByWidth();

    Q_EMIT q->noOfGroupedBytesChanged(noOfGroupedBytes);
}

void ByteArrayRowViewPrivate::setGroupSpacingWidth(int /*PixelX*/ groupSpacingWidth)
{
    if (!mByteArrayColumn->setGroupSpacingWidth(groupSpacingWidth)) {
        return;
    }
    updateViewByWidth();
}

void ByteArrayRowViewPrivate::setBinaryGapWidth(int /*PixelX*/ binaryGapWidth)
{
    if (!mByteArrayColumn->setBinaryGapWidth(binaryGapWidth)) {
        return;
    }
    updateViewByWidth();
}

void ByteArrayRowViewPrivate::setSubstituteChar(QChar substituteChar)
{
    Q_Q(ByteArrayRowView);

    if (!mByteArrayColumn->setSubstituteChar(substituteChar)) {
        return;
    }
    pauseCursor();
    updateColumn(*mByteArrayColumn);
    unpauseCursor();

    Q_EMIT q->substituteCharChanged(substituteChar);
}

void ByteArrayRowViewPrivate::setUndefinedChar(QChar undefinedChar)
{
    Q_Q(ByteArrayRowView);

    if (!mByteArrayColumn->setUndefinedChar(undefinedChar)) {
        return;
    }
    pauseCursor();
    updateColumn(*mByteArrayColumn);
    unpauseCursor();

    Q_EMIT q->undefinedCharChanged(undefinedChar);
}

void ByteArrayRowViewPrivate::setShowsNonprinting(bool showsNonprinting)
{
    Q_Q(ByteArrayRowView);

    if (!mByteArrayColumn->setShowingNonprinting(showsNonprinting)) {
        return;
    }
    pauseCursor();
    updateColumn(*mByteArrayColumn);
    unpauseCursor();

    Q_EMIT q->showsNonprintingChanged(showsNonprinting);
}

void ByteArrayRowViewPrivate::setCharCoding(const QString& newCharCodingName)
{
    Q_Q(ByteArrayRowView);

    if (charCodingName() == newCharCodingName) {
        return;
    }

    AbstractByteArrayViewPrivate::setCharCoding(newCharCodingName);

    pauseCursor();

    mByteArrayColumn->setCharCodec(mCharCodec.get());

    updateColumn(*mByteArrayColumn);

    unpauseCursor();

    Q_EMIT q->charCodecChanged(charCodingName());
}

void ByteArrayRowViewPrivate::setByteTypeColored(bool isColored)
{
    if (isColored == mByteArrayColumn->isByteTypeColored()) {
        return;
    }

    mByteArrayColumn->setByteTypeColored(isColored);

    pauseCursor();
    updateColumn(*mByteArrayColumn);
    unpauseCursor();
}

void ByteArrayRowViewPrivate::adjustTToScaledFont()
{
    // get new values
    const QFontMetrics newFontMetrics(renderingFont());

    mOffsetColumn->setFontMetrics(newFontMetrics);
    mByteArrayColumn->setFontMetrics(newFontMetrics);

    const int rowHeight = mByteArrayColumn->rowHeight();

    setLineHeight(rowHeight);

    // update all dependent structures
    mTableLayout.setNoOfLinesPerPage(noOfLinesPerPage());

    updateViewByWidth();
}

void ByteArrayRowViewPrivate::adjustToLayoutNoOfBytesPerLine()
{
    mByteArrayColumn->resetXBuffer();

    updateWidths();
}

QSize ByteArrayRowViewPrivate::minimumSizeHint() const
{
    Q_Q(const ByteArrayRowView);

    // TODO: better minimal width (visibility!)
    const int minWidth =
        mOffsetColumn->visibleWidth()
        + mOffsetBorderColumn->visibleWidth()
        + mByteArrayColumn->byteWidth();
    const int minHeight =
        lineHeight()
        + noOfLines() > 1 ? q->style()->pixelMetric(QStyle::PM_ScrollBarExtent) : 0;

    return {qMin(minWidth, 100), qMin(minHeight, 100)};
}

int ByteArrayRowViewPrivate::fittingBytesPerLine() const
{
    Q_Q(const ByteArrayRowView);

    const QSize newSize = q->maximumViewportSize();
    const PixelX reservedWidth =
        mOffsetColumn->visibleWidth()
        + mOffsetBorderColumn->visibleWidth();

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
    const PixelX byteWidth = mByteArrayColumn->byteWidth();
    const PixelX byteSpacingWidth = mByteArrayColumn->byteSpacingWidth();
    PixelX groupSpacingWidth;
    int noOfGroupedBytes = mByteArrayColumn->noOfGroupedBytes();
    // no grouping?
    if (noOfGroupedBytes == 0) {
        // fake no grouping by grouping with 1 and using byteSpacingWidth
        noOfGroupedBytes = 1;
        groupSpacingWidth = byteSpacingWidth;
    } else {
        groupSpacingWidth = mByteArrayColumn->groupSpacingWidth();
    }

    const PixelX byteGroupWidth =  noOfGroupedBytes * byteWidth + (noOfGroupedBytes - 1) * byteSpacingWidth;
    const PixelX totalGroupWidth = byteGroupWidth + groupSpacingWidth;

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
//         qCDebug(LOG_OKTETA_GUI) << "Left: " << availableWidth << "("<<byteWidth<<")" ;

            if (availableWidth > 0) {
                fittingBytesPerLine += (availableWidth + byteSpacingWidth) / (byteWidth + byteSpacingWidth);
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
//              << ", h:" << byteGroupWidth
//              << ", s:" << groupSpacingWidth << ") " <<fittingBytesPerLine<< endl;

        const int newNoOfLines = (mTableLayout.length() + mTableLayout.startOffset() + fittingBytesPerLine - 1)
                                 / fittingBytesPerLine;
        const PixelY newHeight =  newNoOfLines * lineHeight();

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
            if (fittingBytesPerLine <= mTableLayout.noOfBytesPerLine()) {
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

void ByteArrayRowViewPrivate::setVisibleCodings(int newCodings)
{
    Q_Q(ByteArrayRowView);

    const int oldCodings = visibleCodings();

    // no changes or no column selected?
    if (newCodings == oldCodings || !(newCodings & AbstractByteArrayView::ValueAndCharCodings)) {
        return;
    }

    mByteArrayColumn->setVisibleCodings(newCodings);

    // active coding not visible anymore?
    const bool isActiveStillVisible = (mActiveCoding & newCodings);
    if (!isActiveStillVisible) {
        mActiveCoding = (AbstractByteArrayView::CodingTypeId)newCodings;
        mInactiveCoding = AbstractByteArrayView::NoCodingId;
        adaptController();
    } else {
        mInactiveCoding = (AbstractByteArrayView::CodingTypeId)(newCodings ^ mActiveCoding);
    }

    const int rowHeight = mByteArrayColumn->rowHeight();
    setLineHeight(rowHeight);

    updateViewByWidth();

    Q_EMIT q->visibleByteArrayCodingsChanged(newCodings);
}

void ByteArrayRowViewPrivate::setActiveCoding(AbstractByteArrayView::CodingTypeId codingId)
{
    // no changes or not visible?
    if (codingId == mActiveCoding) {
        return;
    }

    pauseCursor();
    mValueEditor.finishEdit();

    mActiveCoding = codingId;
    mInactiveCoding = (AbstractByteArrayView::CodingTypeId)(visibleCodings() ^ codingId);

    adaptController();

    ensureCursorVisible();
    unpauseCursor();
}

void ByteArrayRowViewPrivate::placeCursor(QPoint point)
{
    Q_Q(ByteArrayRowView);

    const int lineIndex = lineAt(point.y());
    const PixelY lineY = lineIndex * lineHeight();
    const PixelY y = point.y() - lineY;

    const AbstractByteArrayView::CodingTypeId codingId = mByteArrayColumn->codingIdofY(y);

    // switch active column if needed
    if (codingId != mActiveCoding) {
        mActiveCoding = codingId;
        mInactiveCoding = (codingId != AbstractByteArrayView::ValueCodingId) ? AbstractByteArrayView::ValueCodingId :
                          AbstractByteArrayView::CharCodingId;
    }

    adaptController();

    // get coord of click and whether this click was closer to the end of the pos
    const int linePosition = mByteArrayColumn->magneticLinePositionOfX(point.x());
    const Coord coord(linePosition, lineIndex);

    mTableCursor.gotoCCoord(coord);
    Q_EMIT q->cursorPositionChanged(cursorPosition());
}

Address ByteArrayRowViewPrivate::indexByPoint(QPoint point) const
{
    const int lineIndex = lineAt(point.y());
    const int linePosition = mByteArrayColumn->linePositionOfX(point.x());

    const Coord coord(linePosition, lineIndex);

    return mTableLayout.indexAtCCoord(coord);
}

void ByteArrayRowViewPrivate::blinkCursor()
{
    // skip the cursor drawing?
    if (mCursorPaused || mValueEditor.isInEditMode()) {
        return;
    }

    // switch the cursor state
    mBlinkCursorVisible = !mBlinkCursorVisible;
    updateCursor(*mByteArrayColumn, mActiveCoding);
}

void ByteArrayRowViewPrivate::updateCursors()
{
    createCursorPixmaps();

    mBlinkCursorVisible = true;
    updateCursor(*mByteArrayColumn, mActiveCoding);
    updateCursor(*mByteArrayColumn, mInactiveCoding);
}

void ByteArrayRowViewPrivate::pauseCursor()
{
    mCursorPaused = true;

    mBlinkCursorVisible = false;
    updateCursor(*mByteArrayColumn, mActiveCoding);
    updateCursor(*mByteArrayColumn, mInactiveCoding);
}

QRect ByteArrayRowViewPrivate::cursorRect() const
{
    QRect cursorRect = mByteArrayColumn->byteRect(mTableCursor.coord(), mActiveCoding);
    cursorRect.translate(-xOffset(), -yOffset());

    return cursorRect;
}

// TODO: should be movable to base class
void ByteArrayRowViewPrivate::updateCursor(const ByteArrayRowColumnRenderer& column, AbstractByteArrayView::CodingTypeId codingId)
{
    Q_Q(ByteArrayRowView);

    const bool isCodingVisible = (column.visibleCodings() & codingId);
    if (!isCodingVisible) {
        return;
    }

    QRect cursorRect = column.byteRect(mTableCursor.coord(), codingId);
    cursorRect.translate(-xOffset(), -yOffset());

    q->viewport()->update(cursorRect);
}

void ByteArrayRowViewPrivate::createCursorPixmaps()
{
    Q_Q(ByteArrayRowView);

    const PixelX byteWidth = mByteArrayColumn->byteWidth();

    // create mCursorPixmaps
    mCursorPixmaps.setSize(byteWidth, mByteArrayColumn->digitHeight(), q->devicePixelRatio());

    const Address index = mTableCursor.validIndex();

    QPainter painter;
    painter.begin(&mCursorPixmaps.offPixmap());
    initPainterFromWidget(&painter);
    mByteArrayColumn->renderByte(&painter, index, mActiveCoding);
    painter.end();

    painter.begin(&mCursorPixmaps.onPixmap());
    initPainterFromWidget(&painter);
    mByteArrayColumn->renderCursor(&painter, index, mActiveCoding);
    painter.end();

    // calculate the shape
    PixelX cursorX;
    PixelX cursorW;
    const int insertCursorWidth = q->style()->pixelMetric(QStyle::PM_TextCursorWidth);
    if (isCursorBehind()) {
        cursorX = qMax(0, byteWidth - insertCursorWidth);
        cursorW = insertCursorWidth;
    } else {
        cursorX = 0;
        cursorW = mOverWrite ? -1 : insertCursorWidth;
    }
    mCursorPixmaps.setShape(cursorX, cursorW, q->devicePixelRatio());
}

void ByteArrayRowViewPrivate::drawActiveCursor(QPainter* painter)
{
    Q_Q(ByteArrayRowView);

    // any reason to skip the cursor drawing?
    if (!q->hasFocus() && !q->viewport()->hasFocus() && !mDropper.isActive()) {
        return;
    }

    const int x = mByteArrayColumn->xOfLinePosition(mTableCursor.pos());
    const int y = lineHeight() * mTableCursor.line()
                  + mByteArrayColumn->yOfCodingId(mActiveCoding);

    painter->translate(x, y);

    // paint edited byte?
    if (mValueEditor.isInEditMode()) {
        const Address index = mTableCursor.index();

        if (mBlinkCursorVisible) {
            mByteArrayColumn->renderEditedByte(painter, mValueEditor.value(), mValueEditor.valueAsString());
        } else {
            mByteArrayColumn->renderByte(painter, index, mActiveCoding);
        }
    } else {
        painter->drawPixmap(mCursorPixmaps.cursorX(), 0,
                            mBlinkCursorVisible ? mCursorPixmaps.onPixmap() : mCursorPixmaps.offPixmap(),
                            mCursorPixmaps.shapeX(), 0, mCursorPixmaps.shapeW(), -1);
    }

    painter->translate(-x, -y);
}

void ByteArrayRowViewPrivate::drawInactiveCursor(QPainter* painter)
{
    Q_Q(ByteArrayRowView);

    // any reason to skip the cursor drawing?
    if (mInactiveCoding == AbstractByteArrayView::NoCodingId
        || mCursorPaused
        || (!mCursorPaused && !q->hasFocus() && !q->viewport()->hasFocus() && !mDropper.isActive())) {
        return;
    }

    const Address index = mTableCursor.validIndex();

    const int x = mByteArrayColumn->xOfLinePosition(mTableCursor.pos());
    const int y = lineHeight() * mTableCursor.line()
                  + mByteArrayColumn->yOfCodingId(mInactiveCoding);
    painter->translate(x, y);

    const ByteArrayRowColumnRenderer::FrameStyle frameStyle =
        mTableCursor.isBehind() ?                     ByteArrayRowColumnRenderer::Right :
        (mOverWrite || mValueEditor.isInEditMode()) ? ByteArrayRowColumnRenderer::Frame :
        ByteArrayRowColumnRenderer::Left;
    mByteArrayColumn->renderFramedByte(painter, index, mInactiveCoding, frameStyle);

    painter->translate(-x, -y);
}

void ByteArrayRowViewPrivate::renderColumns(QPainter* painter, int cx, int cy, int cw, int ch)
{
    // TODO: consider moving font zooming into ColumnsView
    painter->setFont(renderingFont());

    AbstractByteArrayViewPrivate::renderColumns(painter, cx, cy, cw, ch);
    // TODO: update non blinking cursors. Should this perhaps be done in the buffercolumn?
    // Then it needs to know about inactive, insideByte and the like... well...
    // perhaps subclassing the buffer columns even more, to CharByteArrayColumnRenderer and ValueByteArrayColumnRenderer?

    if (visibleLines(PixelYRange::fromWidth(cy, ch)).includes(mTableCursor.line())) {
        drawActiveCursor(painter);
        drawInactiveCursor(painter);
    }
}

void ByteArrayRowViewPrivate::updateChanged()
{
    Q_Q(ByteArrayRowView);

    const int xOffset = this->xOffset();
    const PixelXRange Xs = PixelXRange::fromWidth(xOffset, visibleWidth());

    // do updates in offset column
    const LineRange changedOffsetLines = mTableRanges.changedOffsetLines();
    if (!changedOffsetLines.isEmpty()) {
        updateColumn(*mOffsetColumn, changedOffsetLines);
    }

    // collect affected buffer columns
    std::vector<ByteArrayRowColumnRenderer*> dirtyColumns;

    if (mByteArrayColumn->overlaps(Xs)) {
        dirtyColumns.emplace_back(mByteArrayColumn);
        mByteArrayColumn->prepareRendering(Xs);
    }

    // any columns to paint?
    if (!dirtyColumns.empty()) {
        // calculate affected lines/indizes
        const LinePositionRange fullPositions(0, mTableLayout.noOfBytesPerLine() - 1);
        CoordRange visibleRange(fullPositions, visibleLines());

        const int lineHeight = this->lineHeight();
        CoordRange changedRange;
        // as there might be multiple selections on this line redo until no more is changed
        while (getNextChangedRange(&changedRange, visibleRange)) {
            PixelY cy = yOffsetOfLine(changedRange.start().line());

            // only one line?
            if (changedRange.start().line() == changedRange.end().line()) {
                const LinePositionRange changedPositions(changedRange.start().pos(), changedRange.end().pos());
                for (auto* const column : dirtyColumns) {
                    const PixelXRange xPixels = column->xsOfLinePositionsInclSpaces(changedPositions);

                    q->viewport()->update(xPixels.start() - xOffset, cy, xPixels.width(), lineHeight);
                }
            }
            //
            else {
                // first line
                const LinePositionRange firstChangedPositions(changedRange.start().pos(), fullPositions.end());
                for (auto* const column : dirtyColumns) {
                    const PixelXRange XPixels = column->xsOfLinePositionsInclSpaces(firstChangedPositions);

                    q->viewport()->update(XPixels.start() - xOffset, cy, XPixels.width(), lineHeight);
                }

                // at least one full line?
                for (int l = changedRange.start().line() + 1; l < changedRange.end().line(); ++l) {
                    cy += lineHeight;
                    for (auto* const column : dirtyColumns) {
                        const PixelXRange XPixels = column->xsOfLinePositionsInclSpaces(fullPositions);

                        q->viewport()->update(XPixels.start() - xOffset, cy, XPixels.width(), lineHeight);
                    }
                }

                // last line
                cy += lineHeight;
                const LinePositionRange lastChangedPositions(fullPositions.start(), changedRange.end().pos());
                for (auto* const column : dirtyColumns) {
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

    mTableRanges.resetChangedRanges();
}

void ByteArrayRowViewPrivate::ensureCursorVisible()
{
    ensureVisible(*mByteArrayColumn, mTableCursor.coord());
}

void ByteArrayRowViewPrivate::ensureVisible(const AddressRange& range, bool ensureStartVisible)
{
    const CoordRange coords = mTableLayout.coordRangeOfIndizes(range);

    // TODO: this is a make-it-work-hack, better do a smart calculation
    ensureVisible(*mByteArrayColumn, ensureStartVisible ? coords.end() : coords.start());
    ensureVisible(*mByteArrayColumn, ensureStartVisible ? coords.start() : coords.end());
}

void ByteArrayRowViewPrivate::ensureVisible(const ByteArrayRowColumnRenderer& column, Coord coord)
{
    Q_Q(ByteArrayRowView);

    const QRect byteRect = column.byteRect(coord);

    const PixelXRange byteXs = PixelXRange::fromWidth(byteRect.x(), byteRect.width());
    const PixelYRange byteYs = PixelYRange::fromWidth(byteRect.y(), byteRect.height());

    const PixelXRange visibleXs = PixelXRange::fromWidth(xOffset(), visibleWidth());
    const PixelYRange visibleYs = PixelXRange::fromWidth(yOffset(), visibleHeight());

    q->horizontalScrollBar()->setValue(visibleXs.startForInclude(byteXs));
    q->verticalScrollBar()->setValue(visibleYs.startForInclude(byteYs));
}

}
