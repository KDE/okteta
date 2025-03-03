/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTBYTEARRAYCOLUMNRENDERER_HPP
#define OKTETA_ABSTRACTBYTEARRAYCOLUMNRENDERER_HPP

// libcolumnsview
#include "abstractcolumnrenderer.hpp"
#include "linepositionrange.hpp"
#include "linerange.hpp"
// Okteta core
#include <Okteta/Address>

class QPainter;
class QRect;
class QFontMetrics;

namespace Okteta {

class Coord;
class CharCodec;

class ByteArrayTableLayout;
class AbstractByteArrayModel;

class AbstractByteArrayColumnRendererPrivate;

/** base class of all buffer column displayers
 * holds all information about the vertical layout of a buffer column
 * knows how to paint the data and the editing things (focus, cursor, selection)
 * but does not offer
 *
 * @author Friedrich W. H. Kossebau
 */
class OKTETAGUI_EXPORT AbstractByteArrayColumnRenderer : public AbstractColumnRenderer
{
public:
    enum FrameStyle
    {
        Frame,
        Left,
        Right
    };

protected:
    OKTETAGUI_NO_EXPORT explicit AbstractByteArrayColumnRenderer(std::unique_ptr<AbstractByteArrayColumnRendererPrivate>&& dd);

public:
    ~AbstractByteArrayColumnRenderer() override;

public: // AbstractColumnRenderer API
    void renderFirstLine(QPainter* painter, PixelXRange Xs, Line firstLineIndex) override;
    void renderNextLine(QPainter* painter) override;

public:
    void prepareRendering(PixelXRange Xs);

public:
    void renderLinePositions(QPainter* painter, Line lineIndex, LinePositionRange linePositions);
    /** paints a cursor based on the type of the byte.
     * @param painter The QPainter
     * @param byteIndex Index of the byte to paint the cursor for. If -1 a space is used as char.
     */
    void renderCursor(QPainter* painter, Address byteIndex);
    /** paints the byte with background.
     * @param painter The QPainter
     * @param byteIndex Index of the byte to paint. If -1 only the background is painted.
     */
    void renderByte(QPainter* painter, Address byteIndex);
    /** paints the byte with background and a frame around.
     * @param painter The QPainter
     * @param byteIndex Index of the byte to paint the frame for. If -1 a space is used as char.
     * @param style the style of the framing
     */
    void renderFramedByte(QPainter* painter, Address byteIndex, FrameStyle style);

public: // modification access
    /** sets the spacing in the hex column
     * @param byteSpacingWidth spacing between the bytes in pixels
     * @param noOfGroupedBytes numbers of grouped bytes, 0 means no grouping
     * @param groupSpacingWidth spacing between the groups in pixels
     * returns true if there was a change
     */
    bool setSpacing(PixelX byteSpacingWidth, int noOfGroupedBytes = 0, PixelX groupSpacingWidth = 0);
    /** sets the spacing between the bytes in the hex column
     * @param byteSpacingWidth spacing between the bytes in pixels
     * returns true if there was a change
     */
    bool setByteSpacingWidth(PixelX byteSpacingWidth);
    /** sets the number of grouped bytes in the hex column
     * @param noOfGroupedBytes numbers of grouped bytes, 0 means no grouping
     * returns true if there was a change
     */
    bool setNoOfGroupedBytes(int noOfGroupedBytes);
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

    void setByteTypeColored(bool byteTypeColored);

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
    int noOfGroupedBytes() const;

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

private:
    Q_DECLARE_PRIVATE(AbstractByteArrayColumnRenderer)
};

}

#endif
