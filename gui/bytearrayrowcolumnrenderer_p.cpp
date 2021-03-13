/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrowcolumnrenderer_p.hpp"

// lib
#include "bytearraytableranges.hpp"
#include "bytearraytablecursor.hpp"
#include "bytearraytablelayout.hpp"
#include "helper.hpp"
// lib
#include <abstractcolumnstylist.hpp>
// Okteta core
#include <Okteta/BookmarksConstIterator>
#include <Okteta/Bookmark>
#include <Okteta/CharCodec>
#include <Okteta/ValueCodec>
// KF
#include <KColorScheme>
// Qt
#include <QPainter>
#include <QFontMetrics>

namespace Okteta {

AbstractByteArrayView::CodingTypeId ByteArrayRowColumnRendererPrivate::codingIdofY(PixelY y) const
{
    return
        (mVisibleCodings != AbstractByteArrayView::ValueAndCharCodings) ?
            (AbstractByteArrayView::CodingTypeId)mVisibleCodings :
        (y < mDigitHeight) ?
            AbstractByteArrayView::ValueCodingId :
            AbstractByteArrayView::CharCodingId;
}

PixelY ByteArrayRowColumnRendererPrivate::rowHeight() const
{
    const int noOfCodings = (mVisibleCodings > 2) ? 2 : 1;
    return noOfCodings * mDigitHeight + DefaultRowSpacingHeight;
}

PixelY ByteArrayRowColumnRendererPrivate::yOfCodingId(AbstractByteArrayView::CodingTypeId codingId) const
{
    PixelY result = (mVisibleCodings <= 2) ?                             0 :
                    (codingId == AbstractByteArrayView::ValueCodingId) ? 0 :
                                                                         mDigitHeight;

    return result;
}

void ByteArrayRowColumnRendererPrivate::set(AbstractByteArrayModel* byteArrayModel)
{
    mByteArrayModel = byteArrayModel;
    mBookmarks = qobject_cast<Bookmarkable*>(byteArrayModel);
}

void ByteArrayRowColumnRendererPrivate::resetXBuffer()
{
    delete [] mLinePosLeftPixelX;
    delete [] mLinePosRightPixelX;

    mLastLinePos = mLayout->noOfBytesPerLine() - 1;
    mLinePosLeftPixelX =  new PixelX[mLastLinePos + 1];
    mLinePosRightPixelX = new PixelX[mLastLinePos + 1];

    if (mLinePosLeftPixelX) {
        recalcX();
    }
}

void ByteArrayRowColumnRendererPrivate::setVisibleCodings(int visibleCodings)
{
    mVisibleCodings = (AbstractByteArrayView::CodingTypes)(visibleCodings);
}

void ByteArrayRowColumnRendererPrivate::setFontMetrics(const QFontMetrics& fontMetrics)
{
    mFontMetrics = fontMetrics;

    mDigitBaseLine = fontMetrics.ascent();
    mDigitHeight = fontMetrics.height();
    mDigitWidth = fontMetrics.maxWidth();

    // recalculate depend sizes
    recalcByteWidth();

    if (mLinePosLeftPixelX) {
        recalcX();
    }
}

bool ByteArrayRowColumnRendererPrivate::setSpacing(PixelX byteSpacingWidth, Size NoGB, PixelX groupSpacingWidth)
{
    // no changes?
    if (mByteSpacingWidth == byteSpacingWidth && mNoOfGroupedBytes == NoGB && mGroupSpacingWidth == groupSpacingWidth) {
        return false;
    }

    mByteSpacingWidth = byteSpacingWidth;
    mNoOfGroupedBytes = NoGB;
    mGroupSpacingWidth = groupSpacingWidth;

    // recalculate depend sizes
    if (mLinePosLeftPixelX) {
        recalcX();
    }

    return true;
}

bool ByteArrayRowColumnRendererPrivate::setByteSpacingWidth(PixelX byteSpacingWidth)
{
    // no changes?
    if (mByteSpacingWidth == byteSpacingWidth) {
        return false;
    }

    mByteSpacingWidth = byteSpacingWidth;

    // recalculate depend sizes
    if (mLinePosLeftPixelX) {
        recalcX();
    }

    return true;
}

bool ByteArrayRowColumnRendererPrivate::setNoOfGroupedBytes(Size NoGB)
{
    // no changes?
    if (mNoOfGroupedBytes == NoGB) {
        return false;
    }

    mNoOfGroupedBytes = NoGB;

    if (mLinePosLeftPixelX) {
        recalcX();
    }
    return true;
}

bool ByteArrayRowColumnRendererPrivate::setGroupSpacingWidth(PixelX groupSpacingWidth)
{
    // no changes?
    if (mGroupSpacingWidth == groupSpacingWidth) {
        return false;
    }

    mGroupSpacingWidth = groupSpacingWidth;

    // recalculate depend sizes
    if (mLinePosLeftPixelX) {
        recalcX();
    }

    return true;
}

void ByteArrayRowColumnRendererPrivate::setValueCodec(ValueCoding valueCoding, const ValueCodec* valueCodec)
{
    mValueCoding = valueCoding;
    mValueCodec = valueCodec;
    mDecodedByteText.resize(mValueCodec->encodingWidth());

    // recalculate depend sizes
    recalcByteWidth();

    if (mLinePosLeftPixelX) {
        recalcX();
    }
}

bool ByteArrayRowColumnRendererPrivate::setBinaryGapWidth(PixelX binaryGapWidth)
{
    // no changes?
    if (mBinaryGapWidth == binaryGapWidth) {
        return false;
    }

    mBinaryGapWidth = binaryGapWidth;

    // recalculate depend sizes
    recalcByteWidth();

    if (mLinePosLeftPixelX) {
        recalcX();
    }
    return true;
}

void ByteArrayRowColumnRendererPrivate::recalcByteWidth()
{
    // use 0 as reference, using a fixed font should always yield same width
    mValueCodec->encode(&mDecodedByteText, 0, Byte(0));
    if (mValueCoding == BinaryCoding) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
        const int binaryHalfWidth = mFontMetrics.horizontalAdvance(mDecodedByteText.left(4));
#else
        const int binaryHalfWidth = mFontMetrics.width(mDecodedByteText.left(4));
#endif
        mBinaryHalfOffset = binaryHalfWidth + mBinaryGapWidth;
        mByteWidth = mBinaryHalfOffset + binaryHalfWidth;
    } else {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
        mByteWidth = mFontMetrics.horizontalAdvance(mDecodedByteText);
#else
        mByteWidth = mFontMetrics.width(mDecodedByteText);
#endif
    }
}

// perhaps sometimes there will be a grammar
void ByteArrayRowColumnRendererPrivate::renderEditedByte(QPainter* painter, Byte byte, const QString& editBuffer)
{
    const Character byteChar = mCharCodec->decode(byte);

    const QPalette& palette = mStylist->palette();
    KColorScheme colorScheme(palette.currentColorGroup(), KColorScheme::View);
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar) : KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground(foregroundRole);
    painter->fillRect(0, 0, byteWidth(), mDigitHeight, brush);

    const QBrush backgroundBrush = colorScheme.background();
    const QColor& charColor = backgroundBrush.color();
    renderCode(painter, editBuffer, charColor);
}

void ByteArrayRowColumnRendererPrivate::renderByteText(QPainter* painter,
                                                Byte byte, Character byteChar, int codings,
                                                const QColor& color) const
{
    PixelY charBaseLine = mDigitBaseLine;
    if (codings & AbstractByteArrayView::ValueCodingId) {
        mValueCodec->encode(&mDecodedByteText, 0, byte);
        renderCode(painter, mDecodedByteText, color);

        charBaseLine += mDigitHeight;
    }

    if (codings & AbstractByteArrayView::CharCodingId) {
        // turn into a drawable String
        const QString text(byteChar.isUndefined() ?                       Character(mUndefinedChar) :
                           !(mShowingNonprinting || byteChar.isPrint()) ? Character(mSubstituteChar) :
                                                                          byteChar);

        painter->setPen(color);
        painter->drawText(0, charBaseLine, text);
    }
}

void ByteArrayRowColumnRendererPrivate::renderCode(QPainter* painter, const QString& code, const QColor& color) const
{
    painter->setPen(color);
    if (mValueCoding == BinaryCoding) {
        // leave a gap in the middle
        painter->drawText(0, mDigitBaseLine, code.left(4));
        painter->drawText(mBinaryHalfOffset, mDigitBaseLine, code.right(4));
    } else {
        painter->drawText(0, mDigitBaseLine, code);
    }
}

void ByteArrayRowColumnRendererPrivate::recalcX()
{
    Q_Q(ByteArrayRowColumnRenderer);

    mSpacingTrigger = noOfGroupedBytes() > 0 ? noOfGroupedBytes() - 1 : mLastLinePos + 1; // last ensures to never trigger the spacing

    PixelX newWidth = 0;
    Size groupedBytes = 0;
    PixelX* PX = mLinePosLeftPixelX;
    PixelX* PRX = mLinePosRightPixelX;
    LinePosition p = 0;
    for (; p <= mLastLinePos; ++PX, ++PRX, ++p, ++groupedBytes) {
        *PX = newWidth;
        newWidth += mByteWidth;
        *PRX = newWidth - 1;

        // is there a space behind the actual byte (if it is not the last)?
        if (groupedBytes == mSpacingTrigger) {
            newWidth += mGroupSpacingWidth;
            groupedBytes = -1;
        } else {
            newWidth += mByteSpacingWidth;
        }
    }

    q->setWidth(mLinePosRightPixelX[mLastLinePos] + 1);
}

// TODO: why are inlined functions not available as symbols when defined before their use
// TODO: works not precisely for the byte rects but includes spacing and left and right
/*inline*/ LinePosition ByteArrayRowColumnRendererPrivate::linePositionOfX(PixelX PX) const
{
    Q_Q(const ByteArrayRowColumnRenderer);

    if (!mLinePosLeftPixelX) {
        return NoByteFound;
    }

    // translate
    PX -= q->x();
    // search backwards for the first byte that is equalleft to x
    for (LinePosition p = mLastLinePos; p >= 0; --p) {
        if (mLinePosLeftPixelX[p] <= PX) {
            return p;
        }
    }

    return 0; // NoByteFound;
}

LinePosition ByteArrayRowColumnRendererPrivate::magneticLinePositionOfX(PixelX PX) const
{
    Q_Q(const ByteArrayRowColumnRenderer);

    if (!mLinePosLeftPixelX) {
        return NoByteFound;
    }

    // translate
    PX -= q->x();
    // search backwards for the first byte that is equalleft to x
    for (LinePosition p = mLastLinePos; p >= 0; --p) {
        if (mLinePosLeftPixelX[p] <= PX) {
            // are we close to the right?
            if (mLinePosRightPixelX[p] - PX < mDigitWidth / 2) { // TODO: perhaps cache also the middle xpos's
                ++p;
            }
            return p;
        }
    }

    return 0; // NoByteFound;
}

LinePositionRange ByteArrayRowColumnRendererPrivate::linePositionsOfX(PixelX PX, PixelX PW) const
{
    Q_Q(const ByteArrayRowColumnRenderer);

    if (!mLinePosLeftPixelX) {
        return LinePositionRange();
    }

    // translate
    PX -= q->x();
    const PixelX PRX = PX + PW - 1;

    LinePositionRange positions;
    // search backwards for the first byte that is equalleft to x
    for (LinePosition p = mLastLinePos; p >= 0; --p) {
        if (mLinePosLeftPixelX[p] <= PRX) {
            positions.setEnd(p);
            for (; p >= 0; --p) {
                if (mLinePosLeftPixelX[p] <= PX) {
                    positions.setStart(p);
                    break;
                }
            }

            break;
        }
    }

    return positions;
}

PixelX ByteArrayRowColumnRendererPrivate::xOfLinePosition(LinePosition linePosition) const
{
    Q_Q(const ByteArrayRowColumnRenderer);

    return q->x() + (mLinePosLeftPixelX ? mLinePosLeftPixelX[linePosition] : 0);
}

PixelX ByteArrayRowColumnRendererPrivate::rightXOfLinePosition(LinePosition linePosition) const
{
    Q_Q(const ByteArrayRowColumnRenderer);

    return q->x() + (mLinePosRightPixelX ? mLinePosRightPixelX[linePosition] : 0);
}

LinePosition ByteArrayRowColumnRendererPrivate::linePositionOfColumnX(PixelX PX) const
{
    if (!mLinePosLeftPixelX) {
        return NoByteFound;
    }

    // search backwards for the first byte that is equalleft to x
    for (LinePosition p = mLastLinePos; p >= 0; --p) {
        if (mLinePosLeftPixelX[p] <= PX) {
            return p;
        }
    }

    return 0; // NoByteFound;
}

LinePositionRange ByteArrayRowColumnRendererPrivate::linePositionsOfColumnXs(PixelX pixelX, PixelX pixelWidth) const
{
    if (!mLinePosLeftPixelX) {
        return LinePositionRange();
    }

    const PixelX rightPixelX = pixelX + pixelWidth - 1;

    LinePositionRange positions;
    // search backwards for the first byte that is equalleft to x
    for (LinePosition p = mLastLinePos; p >= 0; --p) {
        if (mLinePosLeftPixelX[p] <= rightPixelX) {
            const LinePosition endPos = p;
            positions.setEnd(p);
            for (p = 0; p <= endPos; ++p) {
                if (mLinePosRightPixelX[p] >= pixelX) {
                    positions.setStart(p);
                    break;
                }
            }

            break;
        }
    }

    return positions;
}

PixelX ByteArrayRowColumnRendererPrivate::columnXOfLinePosition(LinePosition linePosition) const
{
    return mLinePosLeftPixelX ? mLinePosLeftPixelX[linePosition] : 0;
}

PixelX ByteArrayRowColumnRendererPrivate::columnRightXOfLinePosition(LinePosition linePosition) const
{
    return mLinePosRightPixelX ? mLinePosRightPixelX[linePosition] : 0;
}

PixelXRange ByteArrayRowColumnRendererPrivate::xsOfLinePositionsInclSpaces(const LinePositionRange& linePositions) const
{
    const PixelX x = (linePositions.start() > 0) ? rightXOfLinePosition(linePositions.nextBeforeStart()) + 1 :
                                                   xOfLinePosition(linePositions.start());
    const PixelX rightX = (linePositions.end() < mLastLinePos) ? xOfLinePosition(linePositions.nextBehindEnd()) - 1 :
                                                                 rightXOfLinePosition(linePositions.end());
    return PixelXRange(x, rightX);
}

PixelXRange ByteArrayRowColumnRendererPrivate::columnXsOfLinePositionsInclSpaces(const LinePositionRange& linePositions) const
{
    const PixelX x = (linePositions.start() > 0) ? columnRightXOfLinePosition(linePositions.nextBeforeStart()) + 1 :
                                                   columnXOfLinePosition(linePositions.start());
    const PixelX rightX = (linePositions.end() < mLastLinePos) ? columnXOfLinePosition(linePositions.nextBehindEnd()) - 1 :
                                                                 columnRightXOfLinePosition(linePositions.end());
    return PixelXRange(x, rightX);
}

QRect ByteArrayRowColumnRendererPrivate::byteRect(Coord coord) const
{
    Q_Q(const ByteArrayRowColumnRenderer);

    const PixelY lineHeight = q->lineHeight();
    const int x = xOfLinePosition(coord.pos());
    const int y = lineHeight * coord.line();
    const int w = mByteWidth;
    const int h = lineHeight;
    const QPoint point(x, y);
    const QSize size(w, h);

    const QRect result(point, size);
    return result;
}

QRect ByteArrayRowColumnRendererPrivate::byteRect(Coord coord, AbstractByteArrayView::CodingTypeId codingId) const
{
    Q_Q(const ByteArrayRowColumnRenderer);

    const int x = xOfLinePosition(coord.pos());
    const int y = q->lineHeight() * coord.line() + yOfCodingId(codingId);
    const int w = mByteWidth;
    const int h = mDigitHeight;
    const QPoint point(x, y);
    const QSize size(w, h);

    const QRect result(point, size);
    return result;
}

void ByteArrayRowColumnRendererPrivate::prepareRendering(const PixelXRange& _Xs)
{
    Q_Q(ByteArrayRowColumnRenderer);

    PixelXRange Xs(_Xs);
    q->restrictToXSpan(&Xs);
    // translate
    Xs.moveBy(-q->x());

    // store the values
    mRenderX = Xs.start();
    mRenderWidth = Xs.width();

    // get line linePositions to paint
    mRenderLinePositions = linePositionsOfColumnXs(mRenderX, mRenderWidth);
}

void ByteArrayRowColumnRendererPrivate::renderFirstLine(QPainter* painter, const PixelXRange& Xs, Line firstLineIndex)
{
    prepareRendering(Xs);

    mRenderLine = firstLineIndex;

    renderLinePositions(painter, mRenderLine++, mRenderLinePositions);
}

void ByteArrayRowColumnRendererPrivate::renderNextLine(QPainter* painter)
{
    renderLinePositions(painter, mRenderLine++, mRenderLinePositions);
}

void ByteArrayRowColumnRendererPrivate::renderLinePositions(QPainter* painter, Line lineIndex, const LinePositionRange& _linePositions)
{
    // clear background
    const unsigned int blankFlag =
        (_linePositions.start() != 0 ? StartsBefore : 0) | (_linePositions.end() != mLastLinePos ? EndsLater : 0);
    const QBrush& backgroundBrush = mStylist->palette().brush(QPalette::Base);

    renderRange(painter, backgroundBrush, _linePositions, blankFlag);

    // no bytes to paint?
    if (!mLayout->hasContent(lineIndex)) {
        return;
    }

    // Go through the lines TODO: handle first and last line more effeciently
    // check for leading and trailing spaces
    const LinePositionRange existingLinePositions = mLayout->linePositions(lineIndex);

    LinePositionRange linePositions = _linePositions;
    linePositions.restrictTo(existingLinePositions);
    const int firstLinePosition = linePositions.start();

    // check for leading and trailing spaces
    AddressRange byteIndizes =
        AddressRange::fromWidth(mLayout->indexAtCoord(Coord(linePositions.start(), lineIndex)), linePositions.width());

    unsigned int selectionFlag = 0;
    unsigned int markingFlag = 0;
    AddressRange selectedRange;
    AddressRange markedRange;
    bool hasMarking = mRanges->hasMarking();
    bool hasSelection = mRanges->hasSelection();

// qCDebug(LOG_OKTETA_GUI) << QString("painting linePositions (painter%1-%2L%3): ").arg(linePositions.start()).arg(linePositions.end()).arg(lineIndex)
//         <<linePositions.start()<<"-"<<linePositions.start()
//         <<" for byteIndizes "<<byteIndizes.start()<<"-"<<byteIndizes.start()<<endl;
    while (linePositions.isValid()) {
        LinePositionRange positionsPart = linePositions;  // set of linePositions to paint next
        AddressRange byteIndizesPart = byteIndizes;      // set of indizes to paint next

        if (hasMarking && markedRange.endsBefore(byteIndizesPart)) {
            hasMarking = getNextMarkedAddressRange(&markedRange, &markingFlag, byteIndizesPart);
        }

        if (hasSelection && selectedRange.endsBefore(byteIndizesPart)) {
            hasSelection = getNextSelectedAddressRange(&selectedRange, &selectionFlag, byteIndizesPart);
        }

        if (byteIndizesPart.start() == markedRange.start()) {
            byteIndizesPart.setEnd(markedRange.end());
            positionsPart.setEndByWidth(markedRange.width());

            if (positionsPart.start() == existingLinePositions.start()) {
                markingFlag &= ~StartsBefore;
            }
            // TODO: hack: needed because otherwise the spacing will be plain
            else if (positionsPart.start() == firstLinePosition && selectedRange.includes(byteIndizesPart.start())) {
                renderSelectionSpaceBehind(painter, firstLinePosition - 1);
            }

            if (positionsPart.end() == existingLinePositions.end()) {
                markingFlag &= ~EndsLater;
            }
            // TODO: hack: needed because otherwise the spacing will be plain
            else if (positionsPart.end() == linePositions.end() && selectedRange.includes(byteIndizesPart.end())) {
                renderSelectionSpaceBehind(painter, positionsPart.end());
            }

            renderMarking(painter, positionsPart, byteIndizesPart.start(), markingFlag);
        } else if (selectedRange.includes(byteIndizesPart.start())) {
            if (selectedRange.startsBefore(byteIndizesPart)) {
                selectionFlag |= StartsBefore;
            }

            const bool hasMarkingBeforeSelectionEnd = (hasMarking && markedRange.start() <= selectedRange.end());

            byteIndizesPart.setEnd(hasMarkingBeforeSelectionEnd ? markedRange.nextBeforeStart() : selectedRange.end());
            positionsPart.setEndByWidth(byteIndizesPart.width());

            if (hasMarkingBeforeSelectionEnd) {
                selectionFlag |= EndsLater;
            }
            if (positionsPart.start() == existingLinePositions.start()) {
                selectionFlag &= ~StartsBefore;
            }
            if (positionsPart.end() == existingLinePositions.end()) {
                selectionFlag &= ~EndsLater;
            }

            renderSelection(painter, positionsPart, byteIndizesPart.start(), selectionFlag);
        } else {
            // calc end of plain text
            if (hasMarking) {
                byteIndizesPart.setEnd(markedRange.nextBeforeStart());
            }
            if (hasSelection) {
                byteIndizesPart.restrictEndTo(selectedRange.nextBeforeStart());
            }

            positionsPart.setEndByWidth(byteIndizesPart.width());

            renderPlain(painter, positionsPart, byteIndizesPart.start());
        }

        byteIndizes.setStartNextBehind(byteIndizesPart);
        linePositions.setStartNextBehind(positionsPart);
    }
}

void ByteArrayRowColumnRendererPrivate::renderPlain(QPainter* painter, const LinePositionRange& linePositions, Address byteIndex)
{
    BookmarksConstIterator bit;
    Address nextBookmarkOffset = -1;

    const bool hasBookmarks = (mBookmarks != nullptr);
    if (hasBookmarks) {
        bit = mBookmarks->createBookmarksConstIterator();
        if (bit.findNextFrom(byteIndex)) {
            nextBookmarkOffset = bit.next().offset();
        }
    }

    const QPalette& palette = mStylist->palette();
    KColorScheme colorScheme(palette.currentColorGroup(), KColorScheme::View);

    // paint all the bytes affected
    for (LinePosition linePosition = linePositions.start(); linePosition <= linePositions.end(); ++linePosition, ++byteIndex) {
        const PixelX x = columnXOfLinePosition(linePosition);

        // draw the byte
        painter->translate(x, 0);

        if (byteIndex == nextBookmarkOffset) {
            renderBookmark(painter, colorScheme.background(KColorScheme::NeutralBackground));

            nextBookmarkOffset = bit.hasNext() ? bit.next().offset() : -1;// TODO )&& ( bit->offset() <= LastIndex );
        }

        const Byte byte = mByteArrayModel->byte(byteIndex);
        const Character byteChar = mCharCodec->decode(byte);

        const KColorScheme::ForegroundRole foregroundRole =
            mByteTypeColored ? foregroundRoleForChar(byteChar) : KColorScheme::NormalText;
        const QBrush brush = colorScheme.foreground(foregroundRole);
        const QColor& charColor = brush.color();// palette.text().color();//colorForChar(byteChar)
        renderByteText(painter, byte, byteChar, mVisibleCodings, charColor);

        painter->translate(-x, 0);
    }
}

void ByteArrayRowColumnRendererPrivate::renderSelection(QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag)
{
    BookmarksConstIterator bit;
    Address nextBookmarkOffset = -1;

    const bool hasBookmarks = (mBookmarks != nullptr);
    if (hasBookmarks) {
        bit = mBookmarks->createBookmarksConstIterator();
        if (bit.findNextFrom(byteIndex)) {
            nextBookmarkOffset = bit.next().offset();
        }
    }

    const QPalette& palette = mStylist->palette();
    KColorScheme colorScheme(palette.currentColorGroup(), KColorScheme::Selection);

    renderRange(painter, colorScheme.background(), linePositions, flag);

    // paint all the bytes affected
    for (LinePosition linePosition = linePositions.start(); linePosition <= linePositions.end(); ++linePosition, ++byteIndex) {
        const PixelX x = columnXOfLinePosition(linePosition);

        // draw the byte
        painter->translate(x, 0);

        if (byteIndex == nextBookmarkOffset) {
            renderBookmark(painter, colorScheme.background(KColorScheme::NeutralBackground));

            nextBookmarkOffset = bit.hasNext() ? bit.next().offset() : -1;// TODO )&& ( bit->offset() <= LastIndex );
        }

        const Byte byte = mByteArrayModel->byte(byteIndex);
        const Character byteChar = mCharCodec->decode(byte);

        const KColorScheme::ForegroundRole foregroundRole =
            mByteTypeColored ? foregroundRoleForChar(byteChar) : KColorScheme::NormalText;
        const QBrush brush = colorScheme.foreground(foregroundRole);
        const QColor& charColor = brush.color();
        renderByteText(painter, byte, byteChar, mVisibleCodings, charColor);

        painter->translate(-x, 0);
    }
}

void ByteArrayRowColumnRendererPrivate::renderSelectionSpaceBehind(QPainter* painter, LinePosition linePosition)
{
    const QPalette& palette = mStylist->palette();
    KColorScheme colorScheme(palette.currentColorGroup(), KColorScheme::Selection);

    renderSpaceBehind(painter, colorScheme.background(), linePosition);
}

void ByteArrayRowColumnRendererPrivate::renderMarking(QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag)
{
    const QPalette& palette = mStylist->palette();

    renderRange(painter, palette.text(), linePositions, flag);

    const QColor& baseColor = palette.base().color();
    // paint all the bytes affected
    for (LinePosition p = linePositions.start(); p <= linePositions.end(); ++p, ++byteIndex) {
        const PixelX x = columnXOfLinePosition(p);

        // draw the byte
        painter->translate(x, 0);
        const Byte byte = mByteArrayModel->byte(byteIndex);
        const Character byteChar = mCharCodec->decode(byte);
        renderByteText(painter, byte, byteChar, mVisibleCodings, baseColor);

        painter->translate(-x, 0);
    }
}

void ByteArrayRowColumnRendererPrivate::renderBookmark(QPainter* painter, const QBrush& brush)
{
    Q_Q(ByteArrayRowColumnRenderer);

    // TODO: think about how bookmarks should really be rendered
    painter->fillRect(1, 1, mByteWidth - 2, q->lineHeight() - 2, brush);
}

void ByteArrayRowColumnRendererPrivate::renderRange(QPainter* painter, const QBrush& brush, const LinePositionRange& linePositions, int flag)
{
    Q_Q(ByteArrayRowColumnRenderer);

    const PixelX rangeX =
        (flag & StartsBefore) ? columnRightXOfLinePosition(linePositions.nextBeforeStart()) + 1 :
                                columnXOfLinePosition(linePositions.start());
    const PixelX rangeW =
        ((flag & EndsLater) ? columnXOfLinePosition(linePositions.nextBehindEnd()) :
                              columnRightXOfLinePosition(linePositions.end()) + 1)
        - rangeX;

    painter->fillRect(rangeX, 0, rangeW, q->lineHeight(), brush);
}

void ByteArrayRowColumnRendererPrivate::renderSpaceBehind(QPainter* painter, const QBrush& brush, LinePosition linePosition)
{
    Q_Q(ByteArrayRowColumnRenderer);

    const PixelX rangeX = columnRightXOfLinePosition(linePosition) + 1;
    const PixelX rangeW = columnXOfLinePosition(linePosition + 1) - rangeX;

    painter->fillRect(rangeX, 0, rangeW, q->lineHeight(), brush);
}

void ByteArrayRowColumnRendererPrivate::renderByte(QPainter* painter,
                                            Address byteIndex, AbstractByteArrayView::CodingTypeId codingId)
{
    const Byte byte = (byteIndex > -1) ? mByteArrayModel->byte(byteIndex) : EmptyByte;
    const Character byteChar = mCharCodec->decode(byte);

    const QPalette& palette = mStylist->palette();

    KColorScheme::ColorSet colorSet = KColorScheme::View;
    if (byteIndex > -1) {
        if (mRanges->selectionIncludes(byteIndex)) {
            colorSet = KColorScheme::Selection;
        }
//    else if( mRanges->markingIncludes(byteIndex) )
//    {
//      charColor = palette.base().color();
//      brush = palette.text();
//    }
    }
    KColorScheme colorScheme(palette.currentColorGroup(), colorSet);

    const QBrush backgroundBrush = colorScheme.background();
    painter->fillRect(0, 0, mByteWidth, mDigitHeight, backgroundBrush);

    if (mBookmarks && mBookmarks->containsBookmarkFor(byteIndex)) {
        const QBrush bookmarkBackgroundBrush = colorScheme.background(KColorScheme::NeutralBackground);
        painter->fillRect(1, 1, mByteWidth - 2, mDigitHeight - 2, bookmarkBackgroundBrush);
    }

    if (byteIndex > -1) {
        const KColorScheme::ForegroundRole foregroundRole =
            mByteTypeColored ? foregroundRoleForChar(byteChar) : KColorScheme::NormalText;
        const QBrush brush = colorScheme.foreground(foregroundRole);
        const QColor& charColor = brush.color();

        renderByteText(painter, byte, byteChar, codingId, charColor);
    }
}

// TODO: think about making framestyle a enum of a class ByteArrayColumnCursor
void ByteArrayRowColumnRendererPrivate::renderFramedByte(QPainter* painter,
                                                  Address byteIndex, AbstractByteArrayView::CodingTypeId codingId,
                                                  ByteArrayRowColumnRenderer::FrameStyle frameStyle)
{
    renderByte(painter, byteIndex, codingId);

    const Byte byte = (byteIndex > -1) ? mByteArrayModel->byte(byteIndex) : EmptyByte;
    const Character byteChar = mCharCodec->decode(byte);

    const bool isInSelection = (byteIndex > -1 && mRanges->selectionIncludes(byteIndex));
    const KColorScheme::ColorSet colorSet = isInSelection ? KColorScheme::Selection : KColorScheme::View;

    const QPalette& palette = mStylist->palette();
    KColorScheme colorScheme(palette.currentColorGroup(), colorSet);
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar) : KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground(foregroundRole);
    QPen pen(brush.color());
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    if (frameStyle == ByteArrayRowColumnRenderer::Frame) {
        painter->drawRect(QRectF(0.5, 0.5, mByteWidth - 1, mDigitHeight - 1));
    } else if (frameStyle == ByteArrayRowColumnRenderer::Left) {
        painter->drawLine(QPointF(0.5, 0.5), QPointF(0.5, mDigitHeight - 0.5));
    } else {
        painter->drawLine(QPointF(mByteWidth - 0.5, 0), QPointF(mByteWidth - 0.5, mDigitHeight - 0.5));
    }
}

void ByteArrayRowColumnRendererPrivate::renderCursor(QPainter* painter, Address byteIndex, AbstractByteArrayView::CodingTypeId codingId)
{
    Q_UNUSED(codingId)
    const Byte byte = (byteIndex > -1) ? mByteArrayModel->byte(byteIndex) : EmptyByte;
    const Character byteChar = mCharCodec->decode(byte);

    const bool isInSelection = (byteIndex > -1 && mRanges->selectionIncludes(byteIndex));
    const KColorScheme::ColorSet colorSet = isInSelection ? KColorScheme::Selection : KColorScheme::View;

    const QPalette& palette = mStylist->palette();
    KColorScheme colorScheme(palette.currentColorGroup(), colorSet);
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar) : KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground(foregroundRole);
    painter->fillRect(0, 0, mByteWidth, mDigitHeight, brush);

    if (byteIndex > -1) {
        const bool isBookmark = (byteIndex > -1 && mBookmarks && mBookmarks->containsBookmarkFor(byteIndex));
        const KColorScheme::BackgroundRole backgroundRole =
            isBookmark ? KColorScheme::NeutralBackground : KColorScheme::NormalBackground;
        const QBrush charBrush = colorScheme.background(backgroundRole);
        const QColor& charColor = charBrush.color();
        renderByteText(painter, byte, byteChar, codingId, charColor);
    }
}

bool ByteArrayRowColumnRendererPrivate::getNextSelectedAddressRange(AddressRange* _selection, unsigned int* _flag,
                                                             const AddressRange& range) const
{
    const AddressRange* overlappingSelectedSection = mRanges->firstOverlappingSelection(range);
    if (!overlappingSelectedSection) {
        return false;
    }

    AddressRange selectedRange = *overlappingSelectedSection;
    unsigned int flag = 0;

    // does selectedRange start before asked range?
    if (selectedRange.startsBefore(range)) {
        selectedRange.setStart(range.start());
        flag |= StartsBefore;
    }

    // does selectedRange go on behind asked range?
    if (selectedRange.endsBehind(range)) {
        selectedRange.setEnd(range.end());
        flag |= EndsLater;
    }

    *_selection = selectedRange;
    *_flag = flag;
    return true;
}

bool ByteArrayRowColumnRendererPrivate::getNextMarkedAddressRange(AddressRange* _markedSection, unsigned int* _flag,
                                                           const AddressRange& range) const
{
    const AddressRange* overlappingMarkedSection = mRanges->overlappingMarking(range);
    if (!overlappingMarkedSection) {
        return false;
    }

    unsigned int flag = 0;
    AddressRange markedRange = *overlappingMarkedSection;

    if (markedRange.startsBefore(range)) {
        markedRange.setStart(range.start());
        flag |= StartsBefore;
    }

    if (markedRange.endsBehind(range)) {
        markedRange.setEnd(range.end());
        flag |= EndsLater;
    }

    *_markedSection = markedRange;
    *_flag = flag;
    return true;
}

}
