/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYROWCOLUMNRENDERER_HPP
#define OKTETA_BYTEARRAYROWCOLUMNRENDERER_HPP

// lib
#include "abstractbytearrayview.hpp"
// libcolumnsview
#include "abstractcolumnrenderer.hpp"
#include "linepositionrange.hpp"
// Okteta core
#include <Okteta/OktetaCore>
#include <Okteta/Byte>

class QFontMetrics;
class QRect;

namespace Okteta {

class Coord;
class CharCodec;
class ValueCodec;

class ByteArrayTableRanges;
class ByteArrayTableLayout;
class AbstractByteArrayModel;

class ByteArrayRowColumnRendererPrivate;

/** base class of all buffer column displayers
 * holds all information about the vertical layout of a buffer column
 * knows how to paint the data and the editing things (focus, cursor, selection)
 * but does not offer
 *
 * @author Friedrich W. H. Kossebau
 */
class OKTETAGUI_EXPORT ByteArrayRowColumnRenderer : public AbstractColumnRenderer
{
public:
    enum FrameStyle
    {
        Frame,
        Left,
        Right
    };

public:
    ByteArrayRowColumnRenderer(AbstractColumnStylist* stylist,
                               AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges);
    ~ByteArrayRowColumnRenderer() override;

public: // AbstractColumnRenderer API
    void renderFirstLine(QPainter* painter, PixelXRange Xs, Line firstLineIndex) override;
    void renderNextLine(QPainter* painter) override;

public:
    void prepareRendering(PixelXRange Xs);

public:
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
    void renderFramedByte(QPainter* painter, Address byteIndex, AbstractByteArrayView::CodingTypeId codingId, FrameStyle style);

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
    [[nodiscard]]
    LinePositionRange linePositionsOfX(PixelX x, PixelX width) const;
    /** returns byte pos at pixel with absolute x-coord x */
    [[nodiscard]]
    LinePosition linePositionOfX(PixelX x) const;
    /** returns byte pos at pixel with absolute x-coord x, and sets the flag to true if we are closer to the right */
    [[nodiscard]]
    LinePosition magneticLinePositionOfX(PixelX x) const;
    /** returns absolute x-coord of byte at position linePosition */
    [[nodiscard]]
    PixelX xOfLinePosition(LinePosition linePosition) const;
    /** returns right absolute x-coord of byte at position linePosition */
    [[nodiscard]]
    PixelX rightXOfLinePosition(LinePosition linePosition) const;
    /** returns byte pos at pixel with relative x-coord x */
    [[nodiscard]]
    LinePosition linePositionOfColumnX(PixelX x) const;
    /** returns byte linePositions covered by pixels with relative x-coord x */
    [[nodiscard]]
    LinePositionRange linePositionsOfColumnXs(PixelX x, PixelX width) const;
    /** returns relative x-coord of byte at position linePosition */
    [[nodiscard]]
    PixelX columnXOfLinePosition(LinePosition linePosition) const;
    /** returns right relative x-coord of byte at position linePosition */
    [[nodiscard]]
    PixelX columnRightXOfLinePosition(LinePosition linePosition) const;
    /** returns the linePositions that overlap with the x-coords relative to the view */
    [[nodiscard]]
    LinePositionRange visibleLinePositions(PixelX x, PixelX width) const;
    /** returns the */
    [[nodiscard]]
    PixelXRange xsOfLinePositionsInclSpaces(LinePositionRange linePositions) const;
    /** */
    [[nodiscard]]
    PixelXRange columnXsOfLinePositionsInclSpaces(LinePositionRange linePositions) const;

    [[nodiscard]]
    QRect byteRect(Coord coord) const;
    [[nodiscard]]
    QRect byteRect(Coord coord, AbstractByteArrayView::CodingTypeId codingId) const;

    [[nodiscard]]
    AbstractByteArrayView::CodingTypeId codingIdofY(PixelY y) const;
    [[nodiscard]]
    PixelY yOfCodingId(AbstractByteArrayView::CodingTypeId codingId) const;

public: // value access
    [[nodiscard]]
    PixelX byteWidth() const;
    [[nodiscard]]
    PixelX digitWidth() const;
    [[nodiscard]]
    PixelX groupSpacingWidth() const;
    [[nodiscard]]
    PixelX byteSpacingWidth() const;
    [[nodiscard]]
    Size noOfGroupedBytes() const;
    [[nodiscard]]
    PixelY digitHeight() const;
    [[nodiscard]]
    PixelY rowHeight() const;

    [[nodiscard]]
    PixelX binaryGapWidth() const;

    /** returns true if "unprintable" chars (>32) are displayed in the char column
     * with their corresponding character, default is false
     */
    [[nodiscard]]
    bool isShowingNonprinting() const;
    /** returns the actually used substitute character for "unprintable" chars, default is '.' */
    [[nodiscard]]
    QChar substituteChar() const;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    [[nodiscard]]
    QChar undefinedChar() const;

    [[nodiscard]]
    LinePosition firstLinePos() const;
    [[nodiscard]]
    LinePosition lastLinePos()  const;
    [[nodiscard]]
    LinePositionRange visibleLinePositions() const;
    [[nodiscard]]
    const ByteArrayTableLayout* tableLayout() const;
    [[nodiscard]]
    bool isByteTypeColored() const;

    [[nodiscard]]
    AbstractByteArrayView::CodingTypes visibleCodings() const;

private:
    Q_DECLARE_PRIVATE(ByteArrayRowColumnRenderer)
};

}

#endif
