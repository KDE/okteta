/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTBYTEARRAYCOLUMNRENDERER_P_HPP
#define OKTETA_ABSTRACTBYTEARRAYCOLUMNRENDERER_P_HPP

// lib
#include "abstractbytearraycolumnrenderer.hpp"
#include "abstractcolumnrenderer_p.hpp"
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

class AbstractByteArrayColumnRendererPrivate : public AbstractColumnRendererPrivate
{
public:
    AbstractByteArrayColumnRendererPrivate(AbstractByteArrayColumnRenderer* q,
                                           AbstractColumnStylist* stylist,
                                           AbstractByteArrayModel* byteArrayModel,
                                           ByteArrayTableLayout* layout,
                                           ByteArrayTableRanges* ranges);

    ~AbstractByteArrayColumnRendererPrivate() override;

public: // AbstractColumnRenderer API
    void renderFirstLine(QPainter* painter, const PixelXRange& Xs, Line firstLineIndex);
    void renderNextLine(QPainter* painter);

public:
    void prepareRendering(const PixelXRange& Xs);

public:
    void renderLinePositions(QPainter* painter, Line lineIndex, const LineRange& linePositions);
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
    void renderFramedByte(QPainter* painter, Address byteIndex, AbstractByteArrayColumnRenderer::FrameStyle style);

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

public: // value access
    PixelX byteWidth() const;
    PixelX digitWidth() const;
    PixelX groupSpacingWidth() const;
    PixelX byteSpacingWidth() const;
    int noOfGroupedBytes() const;

    LinePosition firstLinePos() const;
    LinePosition lastLinePos()  const;
    LinePositionRange visibleLinePositions() const;
    const ByteArrayTableLayout* layout() const;
    bool isByteTypeColored() const;

protected: // API to be redefined
    virtual void renderByteText(QPainter* painter, Byte byte, Character charByte, const QColor& color) const = 0;
    /** default implementation sets byte width to one digit width */
    virtual void recalcByteWidth();

protected:
    void renderPlain(QPainter* painter, const LinePositionRange& linePositions, Address byteIndex);
    void renderSelection(QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag);
    void renderMarking(QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag);
    void renderRange(QPainter* painter, const QBrush& brush, const LinePositionRange& linePositions, int flag);
    void renderSelectionSpaceBehind(QPainter* painter, LinePosition linePosition);
    void renderSpaceBehind(QPainter* painter, const QBrush& brush, LinePosition linePosition);
    void renderBookmark(QPainter* painter, const QBrush& brush);

    void recalcX();

    bool getNextSelectedAddressRange(AddressRange* selectedRange, unsigned int* flag, const AddressRange& range) const;
    bool getNextMarkedAddressRange(AddressRange* markedRange, unsigned int* flag, const AddressRange& range) const;

    void setByteWidth(int byteWidth);

protected:
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

    /** */
    PixelX mDigitWidth = 0;
    /** */
    PixelY mDigitBaseLine = 0;

    QFontMetrics mFontMetrics;

protected: // individual data
    /** total width of byte display in pixel */
    PixelX mByteWidth = 0;
    /** width of inserting cursor in pixel */
//     PixelX mCursorWidth;
    /** size of the line margin */
    PixelX mByteSpacingWidth;
    /** width of spacing in pixel */
    PixelX mGroupSpacingWidth;

    /** number of grouped bytes */
    int mNoOfGroupedBytes;

    /** pointer to array with buffered linePositions (relative to column position)
     * a spacing gets assigned to the left byte -> ...c|c|c |c|c...
     */
    PixelX* mLinePosLeftPixelX = nullptr;
    PixelX* mLinePosRightPixelX = nullptr;
    /** index of right position */
    LinePosition mLastLinePos = 0;

    /** */
    bool mByteTypeColored = true;

protected: // buffering rendering data
    LinePositionRange mRenderLinePositions;
    Line mRenderLine;
    PixelX mRenderX;
    PixelX mRenderWidth;
    int mSpacingTrigger;

protected:
    AbstractByteArrayColumnRenderer* const q_ptr;

private:
    Q_DECLARE_PUBLIC(AbstractByteArrayColumnRenderer)
};

inline AbstractByteArrayColumnRendererPrivate::AbstractByteArrayColumnRendererPrivate(AbstractByteArrayColumnRenderer* q,
                                                                                      AbstractColumnStylist* stylist,
                                                                                      AbstractByteArrayModel* byteArrayModel,
                                                                                      ByteArrayTableLayout* layout,
                                                                                      ByteArrayTableRanges* ranges)
    : AbstractColumnRendererPrivate(stylist)
    , mByteArrayModel(byteArrayModel)
    , mLayout(layout)
    , mRanges(ranges)
    , mBookmarks(qobject_cast<Bookmarkable*>(byteArrayModel))
    , mFontMetrics(QFont())
    , mByteSpacingWidth(DefaultByteSpacingWidth)
    , mGroupSpacingWidth(DefaultGroupSpacingWidth)
    , mNoOfGroupedBytes(DefaultNoOfGroupedBytes)
    , q_ptr(q)
{
}

inline AbstractByteArrayColumnRendererPrivate::~AbstractByteArrayColumnRendererPrivate()
{
    delete [] mLinePosLeftPixelX;
    delete [] mLinePosRightPixelX;
}

inline PixelX AbstractByteArrayColumnRendererPrivate::byteWidth()         const { return mByteWidth; }
inline PixelX AbstractByteArrayColumnRendererPrivate::digitWidth()        const { return mDigitWidth; }
inline PixelX AbstractByteArrayColumnRendererPrivate::byteSpacingWidth()  const { return mByteSpacingWidth; }
inline PixelX AbstractByteArrayColumnRendererPrivate::groupSpacingWidth() const { return mGroupSpacingWidth; }

inline int AbstractByteArrayColumnRendererPrivate::noOfGroupedBytes()      const { return mNoOfGroupedBytes; }

inline LinePosition AbstractByteArrayColumnRendererPrivate::firstLinePos() const { return mRenderLinePositions.start(); }
inline LinePosition AbstractByteArrayColumnRendererPrivate::lastLinePos()  const { return mRenderLinePositions.end(); }
inline LinePositionRange AbstractByteArrayColumnRendererPrivate::visibleLinePositions() const { return mRenderLinePositions; }

inline const ByteArrayTableLayout* AbstractByteArrayColumnRendererPrivate::layout() const { return mLayout; }

inline void AbstractByteArrayColumnRendererPrivate::setCharCodec(const CharCodec* charCodec)
{
    mCharCodec = charCodec;
}

inline void AbstractByteArrayColumnRendererPrivate::setByteTypeColored(bool byteTypeColored)
{
    mByteTypeColored = byteTypeColored;
}
inline bool AbstractByteArrayColumnRendererPrivate::isByteTypeColored() const { return mByteTypeColored; }

inline void AbstractByteArrayColumnRendererPrivate::setByteWidth(int byteWidth) { mByteWidth = byteWidth; }

}

#endif
