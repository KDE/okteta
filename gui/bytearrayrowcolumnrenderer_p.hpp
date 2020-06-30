/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYROWCOLUMNRENDERER_P_HPP
#define OKTETA_BYTEARRAYROWCOLUMNRENDERER_P_HPP

#include "bytearrayrowcolumnrenderer.hpp"
#include "abstractcolumnrenderer_p.hpp"
// lib
#include "oktetagui.hpp"
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/Bookmarkable>
#include <Okteta/Character>
// Qt
#include <QFontMetrics>

class QColor;
class QBrush;

namespace Okteta {

class Bookmarkable;

static constexpr AbstractByteArrayView::CodingTypes DefaultVisibleCodings =
    (AbstractByteArrayView::CodingTypes)(AbstractByteArrayView::ValueCodingId | AbstractByteArrayView::CharCodingId);


class ByteArrayRowColumnRendererPrivate : public AbstractColumnRendererPrivate
{
public:
    ByteArrayRowColumnRendererPrivate(ByteArrayRowColumnRenderer* q,
                                      AbstractColumnStylist* stylist,
                                      AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout,
                                      ByteArrayTableRanges* ranges);

    ~ByteArrayRowColumnRendererPrivate() override;

public: // AbstractColumnRenderer API
    void renderFirstLine(QPainter* painter, const PixelXRange& Xs, Line firstLineIndex);
    void renderNextLine(QPainter* painter);

public:
    void prepareRendering(const PixelXRange& Xs);

public:
    // void renderLine( QPainter* painter, int lineIndex );
    void renderLinePositions(QPainter* painter, Line lineIndex, const LinePositionRange& linePositions);
    /** paints a cursor based on the type of the byte.
     * @param painter The QPainter.
     * @param byteIndex Index of the byte to paint the cursor for. If -1 a space is used as char.
     * @param codingId The coding identifier.
     */
    void renderCursor(QPainter* painter, Address byteIndex, AbstractByteArrayView::CodingTypeId codingId);
    /** paints the byte with background.
     * @param painter The QPainter.
     * @param byteIndex Index of the byte to paint. If -1 only the background is painted.
     * @param codingId The coding identifier.
     */
    void renderByte(QPainter* painter, Address byteIndex, AbstractByteArrayView::CodingTypeId codingId);
    /** paints the byte with background and a frame around.
     * @param painter The QPainter.
     * @param byteIndex Index of the byte to paint the frame for. If -1 a space is used as char.
     * @param codingId The coding identifier.
     * @param style the style of the framing
     */
    void renderFramedByte(QPainter* painter, Address byteIndex, AbstractByteArrayView::CodingTypeId codingId, ByteArrayRowColumnRenderer::FrameStyle style);

    void renderEditedByte(QPainter* painter, Byte byte, const QString& editBuffer);

public: // modification access
    /** sets the spacing in the hex column
     * @param byteSpacingWidth spacing between the bytes in pixels
     * @param noOfGroupedBytes numbers of grouped bytes, 0 means no grouping
     * @param groupSpacingWidth spacing between the groups in pixels
     * returns true if there was a change
     */
    bool setSpacing(PixelX byteSpacingWidth, Size noOfGroupedBytes = 0, PixelX groupSpacingWidth = 0);
    /** sets the spacing between the bytes in the hex column
     * @param byteSpacingWidth spacing between the bytes in pixels
     * returns true if there was a change
     */
    bool setByteSpacingWidth(PixelX byteSpacingWidth);
    /** sets the number of grouped bytes in the hex column
     * @param noOfGroupedBytes numbers of grouped bytes, 0 means no grouping
     * returns true if there was a change
     */
    bool setNoOfGroupedBytes(Size noOfGroupedBytes);
    /** sets the spacing between the groups of bytes in the hex column
     * @param groupSpacingWidth spacing between the groups in pixels
     * returns true if there was a change
     */
    bool setGroupSpacingWidth(PixelX groupSpacingWidth);
    /** sets the metrics of the used font
     */
    void setFontMetrics(const QFontMetrics& fontMetrics);
    /** */
    void set(AbstractByteArrayModel* byteArrayModel);
    /** creates new buffer for x-values; to be called on any change of NoOfBytesPerLine or metrics */
    void resetXBuffer();
    /** sets the codec to be used by the char column. */
    void setCharCodec(const CharCodec* charCodec);

    void setValueCodec(ValueCoding valueCoding, const ValueCodec* valueCodec);
    /** sets the spacing in the middle of a binary byte in the value column
     * @param binaryGapWidth spacing in the middle of a binary in pixels
     * returns true if there was a change
     */
    bool setBinaryGapWidth(PixelX binaryGapWidth);

    void setByteTypeColored(bool byteTypeColored);

    /** sets whether "unprintable" chars (>32) should be displayed in the char column
     * with their corresponding character.
     * @param showingNonprinting
     * returns true if there was a change
     */
    bool setShowingNonprinting(bool showingNonprinting = true);
    /** sets the substitute character for "unprintable" chars
     * returns true if there was a change
     */
    bool setSubstituteChar(QChar substituteChar);
    /** sets the undefined character for "undefined" chars
     * returns true if there was a change
     */
    bool setUndefinedChar(QChar undefinedChar);

    void setVisibleCodings(int visibleCodings);

public: // functional logic
    /** returns byte linePositions covered by pixels with absolute x-coord x */
    LinePositionRange linePositionsOfX(PixelX x, PixelX width) const;
    /** returns byte pos at pixel with absolute x-coord x */
    LinePosition linePositionOfX(PixelX x) const;
    /** returns byte pos at pixel with absolute x-coord x, and sets the flag to true if we are closer to the right */
    LinePosition magneticLinePositionOfX(PixelX x) const;
    /** returns absolute x-coord of byte at position posInLine */
    PixelX xOfLinePosition(LinePosition posInLine) const;
    /** returns right absolute x-coord of byte at position posInLine */
    PixelX rightXOfLinePosition(LinePosition posInLine) const;
    /** returns byte pos at pixel with relative x-coord x */
    LinePosition linePositionOfColumnX(PixelX x) const;
    /** returns byte linePositions covered by pixels with relative x-coord x */
    LinePositionRange linePositionsOfColumnXs(PixelX x, PixelX width) const;
    /** returns relative x-coord of byte at position posInLine */
    PixelX columnXOfLinePosition(LinePosition posInLine) const;
    /** returns right relative x-coord of byte at position posInLine */
    PixelX columnRightXOfLinePosition(LinePosition posInLine) const;
    /** returns the linePositions that overlap with the x-coords relative to the view */
    LinePositionRange visibleLinePositions(PixelX x, PixelX width) const;
    /** returns the */
    PixelXRange xsOfLinePositionsInclSpaces(const LinePositionRange& linePositions) const;
    /** */
    PixelXRange columnXsOfLinePositionsInclSpaces(const LinePositionRange& linePositions) const;

    QRect byteRect(Coord coord) const;
    QRect byteRect(Coord coord, AbstractByteArrayView::CodingTypeId codingId) const;

    AbstractByteArrayView::CodingTypeId codingIdofY(PixelY y) const;
    PixelY yOfCodingId(AbstractByteArrayView::CodingTypeId codingId) const;

public: // value access
    PixelX byteWidth() const;
    PixelX digitWidth() const;
    PixelX groupSpacingWidth() const;
    PixelX byteSpacingWidth() const;
    Size noOfGroupedBytes() const;
    PixelY digitHeight() const;
    PixelY rowHeight() const;

    PixelX binaryGapWidth() const;

    /** returns true if "unprintable" chars (>32) are displayed in the char column
     * with their corresponding character, default is false
     */
    bool isShowingNonprinting() const;
    /** returns the actually used substitute character for "unprintable" chars, default is '.' */
    QChar substituteChar() const;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    QChar undefinedChar() const;

    LinePosition firstLinePos() const;
    LinePosition lastLinePos()  const;
    LinePositionRange visibleLinePositions() const;
    const ByteArrayTableLayout* layout() const;
    bool isByteTypeColored() const;

    AbstractByteArrayView::CodingTypes visibleCodings() const;

private: // API to be redefined
    void renderByteText(QPainter* painter,
                        Byte byte, Character charByte, int codings, const QColor& color) const;
    /** default implementation sets byte width to one digit width */
    void recalcByteWidth();

private:
    void renderPlain(QPainter* painter, const LinePositionRange& linePositions, Address byteIndex);
    void renderSelection(QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag);
    void renderMarking(QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag);
    void renderRange(QPainter* painter, const QBrush& brush, const LinePositionRange& linePositions, int flag);
    void renderSelectionSpaceBehind(QPainter* painter, LinePosition linePosition);
    void renderSpaceBehind(QPainter* painter, const QBrush& brush, LinePosition linePosition);
    void renderBookmark(QPainter* painter, const QBrush& brush);

    void renderCode(QPainter* painter, const QString& code, const QColor& color) const;

    void recalcX();

    bool getNextSelectedAddressRange(AddressRange* selectedRange, unsigned int* flag, const AddressRange& range) const;
    bool getNextMarkedAddressRange(AddressRange* markedRange, unsigned int* flag, const AddressRange& range) const;

private:
    /** pointer to the buffer */
    AbstractByteArrayModel* mByteArrayModel;
    /** pointer to the layout */
    const ByteArrayTableLayout* mLayout;
    /** pointer to the ranges */
    ByteArrayTableRanges* mRanges;
    /** */
    Bookmarkable* mBookmarks;
    /** */
    const CharCodec* mCharCodec;

    AbstractByteArrayView::CodingTypes mVisibleCodings;
    /** */
    PixelX mDigitWidth = 0;
    /** */
    PixelY mDigitBaseLine = 0;
    /** */
    PixelY mDigitHeight = 0;

    QFontMetrics mFontMetrics;

private: // individual data
    /** total width of byte display in pixel */
    PixelX mByteWidth = 0;
    /** width of inserting cursor in pixel */
//     PixelX mCursorWidth;
    /** size of the line margin */
    PixelX mByteSpacingWidth;
    /** width of spacing in pixel */
    PixelX mGroupSpacingWidth;

    /** number of grouped bytes */
    Size mNoOfGroupedBytes;

    /** pointer to array with buffered linePositions (relative to column position)
     * any spacing gets assigned to the byte left to it -> ...|c|c|c |c|c...
     */
    PixelX* mLinePosLeftPixelX = nullptr;
    PixelX* mLinePosRightPixelX = nullptr;
    /** index of right position */
    LinePosition mLastLinePos = 0;

    /** */
    bool mByteTypeColored = true;

private: // value specifics
    /** */
    ValueCoding mValueCoding;
    /** */
    const ValueCodec* mValueCodec = nullptr;
    /** */
    PixelX mBinaryGapWidth;

private: // buffered data
    /** buffer to hold the formatted valueCoding */
    mutable QString mDecodedByteText;
    /** calculated: Offset in pixels of the second half of the binary */
    PixelX mBinaryHalfOffset;

private: // char specifics
    /** */
    bool mShowingNonprinting;
    /** */
    QChar mSubstituteChar;
    /** */
    QChar mUndefinedChar;

private: // buffering rendering data
    LinePositionRange mRenderLinePositions;
    Line mRenderLine;
    PixelX mRenderX;
    PixelX mRenderWidth;
    int mSpacingTrigger;

private:
    ByteArrayRowColumnRenderer* const q_ptr;
    Q_DECLARE_PUBLIC(ByteArrayRowColumnRenderer)
};


inline ByteArrayRowColumnRendererPrivate::ByteArrayRowColumnRendererPrivate(ByteArrayRowColumnRenderer* q,
                                                                            AbstractColumnStylist* stylist,
                                                                            AbstractByteArrayModel* byteArrayModel,
                                                                            ByteArrayTableLayout* layout,
                                                                            ByteArrayTableRanges* ranges)
    : AbstractColumnRendererPrivate(stylist)
    , mByteArrayModel(byteArrayModel)
    , mLayout(layout)
    , mRanges(ranges)
    , mBookmarks(qobject_cast<Bookmarkable*>(byteArrayModel))
    , mVisibleCodings(DefaultVisibleCodings)
    , mFontMetrics(QFont())
    , mByteSpacingWidth(DefaultByteSpacingWidth)
    , mGroupSpacingWidth(DefaultGroupSpacingWidth)
    , mNoOfGroupedBytes(DefaultNoOfGroupedBytes)
    , mBinaryGapWidth(DefaultBinaryGapWidth)
    , mShowingNonprinting(DefaultShowingNonprinting)
    , mSubstituteChar(DefaultSubstituteChar)
    , mUndefinedChar(DefaultUndefinedChar)
    , q_ptr(q)
{
}

inline ByteArrayRowColumnRendererPrivate::~ByteArrayRowColumnRendererPrivate()
{
    delete [] mLinePosLeftPixelX;
    delete [] mLinePosRightPixelX;
}

inline AbstractByteArrayView::CodingTypes ByteArrayRowColumnRendererPrivate::visibleCodings() const { return mVisibleCodings; }

inline PixelX ByteArrayRowColumnRendererPrivate::byteWidth()         const { return mByteWidth; }
inline PixelX ByteArrayRowColumnRendererPrivate::digitWidth()        const { return mDigitWidth; }
inline PixelX ByteArrayRowColumnRendererPrivate::byteSpacingWidth()  const { return mByteSpacingWidth; }
inline PixelX ByteArrayRowColumnRendererPrivate::groupSpacingWidth() const { return mGroupSpacingWidth; }
inline PixelY ByteArrayRowColumnRendererPrivate::digitHeight()       const { return mDigitHeight; }
inline Size ByteArrayRowColumnRendererPrivate::noOfGroupedBytes()      const { return mNoOfGroupedBytes; }

inline LinePosition ByteArrayRowColumnRendererPrivate::firstLinePos() const { return mRenderLinePositions.start(); }
inline LinePosition ByteArrayRowColumnRendererPrivate::lastLinePos()  const { return mRenderLinePositions.end(); }
inline LinePositionRange ByteArrayRowColumnRendererPrivate::visibleLinePositions() const { return mRenderLinePositions; }

inline const ByteArrayTableLayout* ByteArrayRowColumnRendererPrivate::layout() const { return mLayout; }

inline void ByteArrayRowColumnRendererPrivate::setCharCodec(const CharCodec* charCodec)
{
    mCharCodec = charCodec;
}

inline void ByteArrayRowColumnRendererPrivate::setByteTypeColored(bool byteTypeColored)
{
    mByteTypeColored = byteTypeColored;
}
inline bool ByteArrayRowColumnRendererPrivate::isByteTypeColored() const { return mByteTypeColored; }

inline PixelX ByteArrayRowColumnRendererPrivate::binaryGapWidth()                 const { return mBinaryGapWidth; }

inline bool ByteArrayRowColumnRendererPrivate::isShowingNonprinting()  const { return mShowingNonprinting; }
inline QChar ByteArrayRowColumnRendererPrivate::substituteChar()       const { return mSubstituteChar; }
inline QChar ByteArrayRowColumnRendererPrivate::undefinedChar()        const { return mUndefinedChar; }

inline bool ByteArrayRowColumnRendererPrivate::setSubstituteChar(QChar substituteChar)
{
    if (mSubstituteChar == substituteChar) {
        return false;
    }

    mSubstituteChar = substituteChar;
    return true;
}

inline bool ByteArrayRowColumnRendererPrivate::setUndefinedChar(QChar undefinedChar)
{
    if (mUndefinedChar == undefinedChar) {
        return false;
    }

    mUndefinedChar = undefinedChar;
    return true;
}

inline bool ByteArrayRowColumnRendererPrivate::setShowingNonprinting(bool showingNonprinting)
{
    if (mShowingNonprinting == showingNonprinting) {
        return false;
    }

    mShowingNonprinting = showingNonprinting;
    return true;
}

}

#endif
