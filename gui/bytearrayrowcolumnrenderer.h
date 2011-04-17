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

#ifndef OKTETA_BYTEARRAYROWCOLUMNRENDERER_H
#define OKTETA_BYTEARRAYROWCOLUMNRENDERER_H

// lib
#include "abstractbytearrayview.h"
// ColumnsView
#include "abstractcolumnrenderer.h"
#include "linepositionrange.h"
// Okteta core
#include "oktetacore.h"
#include "abstractbytearraymodel.h"
#include "character.h"
// Qt
#include <QtGui/QFontMetrics>

class QPainter;
class QColor;
class QBrush;
class QRect;


namespace Okteta
{
class Coord;
class Bookmarkable;
class CharCodec;
class ValueCodec;

// class KByteArrayView;
class ByteArrayTableRanges;
class ByteArrayTableLayout;

/** base class of all buffer column displayers
  * holds all information about the vertical layout of a buffer column
  * knows how to paint the data and the editing things (focus, cursor, selection)
  * but does not offer
  *
  *@author Friedrich W. H. Kossebau
  */
class OKTETAGUI_EXPORT ByteArrayRowColumnRenderer : public AbstractColumnRenderer
{
  public:
    enum FrameStyle { Frame, Left, Right };
  public:
    ByteArrayRowColumnRenderer( AbstractColumnStylist* stylist,
        AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges );
    virtual ~ByteArrayRowColumnRenderer();


  public: // AbstractColumnRenderer API
    virtual void renderFirstLine( QPainter* painter, const PixelXRange& Xs, Line firstLineIndex );
    virtual void renderNextLine( QPainter* painter );

  public:
    void prepareRendering( const PixelXRange &Xs );

  public:
    //void renderLine( QPainter* painter, int lineIndex );
    void renderLinePositions( QPainter* painter, Line lineIndex, const LinePositionRange& linePositions );
    /** paints a cursor based on the type of the byte.
      * @param byteIndex Index of the byte to paint the cursor for. If -1 a space is used as char.
      */
    void renderCursor( QPainter* painter, Address byteIndex, AbstractByteArrayView::CodingTypeId codingId );
    /** paints the byte with background.
      * @param byteIndex Index of the byte to paint. If -1 only the background is painted.
      */
    void renderByte( QPainter* painter, Address byteIndex, AbstractByteArrayView::CodingTypeId codingId );
    /** paints the byte with background and a frame around.
      * @param byteIndex Index of the byte to paint the frame for. If -1 a space is used as char.
      * @param style the style of the framing
      */
    void renderFramedByte( QPainter* painter, Address byteIndex, AbstractByteArrayView::CodingTypeId codingId, FrameStyle style );

    void renderEditedByte( QPainter* painter, Byte byte, const QString& editBuffer );

  public: // modification access
    /** sets the spacing in the hex column
      * @param byteSpacingWidth spacing between the bytes in pixels
      * @param noOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * @param groupSpacingWidth spacing between the groups in pixels
      * returns true if there was a change
      */
    bool setSpacing( PixelX byteSpacingWidth, Size noOfGroupedBytes = 0, PixelX groupSpacingWidth = 0 );
    /** sets the spacing between the bytes in the hex column
      * @param byteSpacingWidth spacing between the bytes in pixels
      * returns true if there was a change
      */
    bool setByteSpacingWidth( PixelX byteSpacingWidth );
    /** sets the number of grouped bytes in the hex column
      * @param noOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * returns true if there was a change
      */
    bool setNoOfGroupedBytes( Size noOfGroupedBytes );
    /** sets the spacing between the groups of bytes in the hex column
      * @param groupSpacingWidth spacing between the groups in pixels
      * returns true if there was a change
      */
    bool setGroupSpacingWidth( PixelX groupSpacingWidth );
    /** sets the metrics of the used font
      */
    void setFontMetrics( const QFontMetrics& fontMetrics );
    /** */
    void set( AbstractByteArrayModel* byteArrayModel );
    /** creates new buffer for x-values; to be called on any change of NoOfBytesPerLine or metrics */
    void resetXBuffer();
    /** sets the codec to be used by the char column. */
    void setCharCodec( const CharCodec* charCodec );

    void setValueCodec( ValueCoding valueCoding, const ValueCodec* valueCodec );
    /** sets the spacing in the middle of a binary byte in the value column
      * @param binaryGapWidth spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    bool setBinaryGapWidth( PixelX binaryGapWidth );

    void setByteTypeColored( bool byteTypeColored );

    /** sets whether "unprintable" chars (>32) should be displayed in the char column
      * with their corresponding character.
      * @param showingNonprinting
      * returns true if there was a change
      */
    bool setShowingNonprinting( bool showingNonprinting = true );
    /** sets the substitute character for "unprintable" chars
      * returns true if there was a change
      */
    bool setSubstituteChar( QChar substituteChar );
    /** sets the undefined character for "undefined" chars
      * returns true if there was a change
      */
    bool setUndefinedChar( QChar undefinedChar );

    void setVisibleCodings( int visibleCodings );


  public: // functional logic
    /** returns byte linePositions covered by pixels with absolute x-coord x */
    LinePositionRange linePositionsOfX( PixelX x, PixelX width ) const;
    /** returns byte pos at pixel with absolute x-coord x */
    LinePosition linePositionOfX( PixelX x ) const;
    /** returns byte pos at pixel with absolute x-coord x, and sets the flag to true if we are closer to the right */
    LinePosition magneticLinePositionOfX( PixelX x ) const;
    /** returns absolute x-coord of byte at position posInLine */
    PixelX xOfLinePosition( LinePosition posInLine ) const;
    /** returns right absolute x-coord of byte at position posInLine */
    PixelX rightXOfLinePosition( LinePosition posInLine ) const;
    /** returns byte pos at pixel with relative x-coord x */
    LinePosition linePositionOfColumnX( PixelX x ) const;
    /** returns byte linePositions covered by pixels with relative x-coord x */
    LinePositionRange linePositionsOfColumnXs( PixelX x, PixelX width ) const;
    /** returns relative x-coord of byte at position posInLine */
    PixelX columnXOfLinePosition( LinePosition posInLine ) const;
    /** returns right relative x-coord of byte at position posInLine */
    PixelX columnRightXOfLinePosition( LinePosition posInLine ) const;
    /** returns the linePositions that overlap with the x-coords relative to the view */
    LinePositionRange visibleLinePositions( PixelX x, PixelX width ) const;
    /** returns the */
    PixelXRange xsOfLinePositionsInclSpaces( const LinePositionRange& linePositions ) const;
    /** */
    PixelXRange columnXsOfLinePositionsInclSpaces( const LinePositionRange& linePositions ) const;

    QRect byteRect( const Coord& coord ) const;
    QRect byteRect( const Coord& coord, AbstractByteArrayView::CodingTypeId codingId ) const;

    AbstractByteArrayView::CodingTypeId codingIdofY( PixelY y ) const;
    PixelY yOfCodingId( AbstractByteArrayView::CodingTypeId codingId ) const;

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


  protected: // API to be redefined
    void renderByteText( QPainter* painter,
                         Byte byte, Character charByte, int codings, const QColor& color ) const;
    /** default implementation sets byte width to one digit width */
    void recalcByteWidth();


  protected:
    void renderPlain( QPainter* painter, const LinePositionRange& linePositions, Address byteIndex );
    void renderSelection( QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag );
    void renderMarking( QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag );
    void renderRange( QPainter* painter, const QBrush& brush, const LinePositionRange& linePositions, int flag );
    void renderSelectionSpaceBehind( QPainter* painter, LinePosition linePosition );
    void renderSpaceBehind( QPainter* painter, const QBrush& brush, LinePosition linePosition );
    void renderBookmark( QPainter* painter, const QBrush& brush );

    void renderCode( QPainter* painter, const QString& code, const QColor& color ) const;

    void recalcX();

    bool getNextSelectedAddressRange( AddressRange* selectedRange, unsigned int* flag, const AddressRange& range ) const;
    bool getNextMarkedAddressRange( AddressRange* markedRange, unsigned int* flag, const AddressRange& range ) const;


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

    AbstractByteArrayView::CodingTypes mVisibleCodings;
    /** */
    PixelX mDigitWidth;
    /** */
    PixelY mDigitBaseLine;
    /** */
    PixelY mDigitHeight;

    QFontMetrics mFontMetrics;


  protected:  // individual data
    /** total width of byte display in pixel */
    PixelX mByteWidth;
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
    PixelX* mLinePosLeftPixelX;
    PixelX* mLinePosRightPixelX;
    /** index of right position */
    LinePosition mLastLinePos;

    /** */
    bool mByteTypeColored;

  protected: // value specifics
    /** */
    ValueCoding mValueCoding;
    /** */
    const ValueCodec* mValueCodec;
    /** */
    PixelX mBinaryGapWidth;

  protected: // buffered data
    /** buffer to hold the formatted valueCoding */
    mutable QString mDecodedByteText;
    /** calculated: Offset in pixels of the second half of the binary */
    PixelX mBinaryHalfOffset;

  protected: // char specifics
    /** */
    bool mShowingNonprinting;
    /** */
    QChar mSubstituteChar;
    /** */
    QChar mUndefinedChar;

  protected: // buffering rendering data
    LinePositionRange mRenderLinePositions;
    Line mRenderLine;
    PixelX mRenderX;
    PixelX mRenderWidth;
    int mSpacingTrigger;
};

inline AbstractByteArrayView::CodingTypes ByteArrayRowColumnRenderer::visibleCodings() const { return mVisibleCodings; }


inline PixelX ByteArrayRowColumnRenderer::byteWidth()         const { return mByteWidth; }
inline PixelX ByteArrayRowColumnRenderer::digitWidth()        const { return mDigitWidth; }
inline PixelX ByteArrayRowColumnRenderer::byteSpacingWidth()  const { return mByteSpacingWidth; }
inline PixelX ByteArrayRowColumnRenderer::groupSpacingWidth() const { return mGroupSpacingWidth; }
inline PixelY ByteArrayRowColumnRenderer::digitHeight()       const { return mDigitHeight; }
inline Size ByteArrayRowColumnRenderer::noOfGroupedBytes()      const { return mNoOfGroupedBytes; }

inline LinePosition ByteArrayRowColumnRenderer::firstLinePos() const { return mRenderLinePositions.start(); }
inline LinePosition ByteArrayRowColumnRenderer::lastLinePos()  const { return mRenderLinePositions.end(); }
inline LinePositionRange ByteArrayRowColumnRenderer::visibleLinePositions() const { return mRenderLinePositions; }

inline const ByteArrayTableLayout *ByteArrayRowColumnRenderer::layout() const { return mLayout; }

inline void ByteArrayRowColumnRenderer::setCharCodec( const CharCodec* charCodec )
{
    mCharCodec = charCodec;
}

inline void ByteArrayRowColumnRenderer::setByteTypeColored( bool byteTypeColored )
{
    mByteTypeColored = byteTypeColored;
}
inline bool ByteArrayRowColumnRenderer::isByteTypeColored() const { return mByteTypeColored; }

inline PixelX ByteArrayRowColumnRenderer::binaryGapWidth()                 const { return mBinaryGapWidth; }

inline bool ByteArrayRowColumnRenderer::isShowingNonprinting()  const { return mShowingNonprinting; }
inline QChar ByteArrayRowColumnRenderer::substituteChar()       const { return mSubstituteChar; }
inline QChar ByteArrayRowColumnRenderer::undefinedChar()        const { return mUndefinedChar; }

inline bool ByteArrayRowColumnRenderer::setSubstituteChar( QChar substituteChar )
{
    if( mSubstituteChar == substituteChar )
        return false;

    mSubstituteChar = substituteChar;
    return true;
}

inline bool ByteArrayRowColumnRenderer::setUndefinedChar( QChar undefinedChar )
{
    if( mUndefinedChar == undefinedChar )
        return false;

    mUndefinedChar = undefinedChar;
    return true;
}

inline bool ByteArrayRowColumnRenderer::setShowingNonprinting( bool showingNonprinting )
{
    if( mShowingNonprinting == showingNonprinting )
        return false;

    mShowingNonprinting = showingNonprinting;
    return true;
}

}

#endif
