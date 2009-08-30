/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <abstractcolumnrenderer.h>
// Okteta core
#include <oktetacore.h>
#include <abstractbytearraymodel.h>
#include <character.h>
// commonlib
#include <section.h>


class QPainter;
class QColor;
class QBrush;

namespace Okteta
{

class Bookmarkable;
class CharCodec;
class ValueCodec;

// class KByteArrayView;
class ByteArrayTableRanges;
class ByteArrayTableLayout;

const Address NoByteFound = -1;

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
    virtual void renderFirstLine( QPainter* painter, const KPixelXs& Xs, int firstLineIndex );
    virtual void renderNextLine( QPainter* painter );

  public:
    void prepareRendering( const KPixelXs &Xs );

  public:
    //void renderLine( QPainter* painter, int lineIndex );
    void renderLinePositions( QPainter* painter, int lineIndex, const KDE::Section& linePositions );
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
      * @param Style the style of the framing
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
    bool setSpacing( KPixelX byteSpacingWidth, int noOfGroupedBytes = 0, KPixelX groupSpacingWidth = 0 );
    /** sets the spacing between the bytes in the hex column
      * @param byteSpacingWidth spacing between the bytes in pixels
      * returns true if there was a change
      */
    bool setByteSpacingWidth( KPixelX byteSpacingWidth );
    /** sets the number of grouped bytes in the hex column
      * @param NewNoOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * returns true if there was a change
      */
    bool setNoOfGroupedBytes( int noOfGroupedBytes );
    /** sets the spacing between the groups of bytes in the hex column
      * @param GroupSpacingW spacing between the groups in pixels
      * returns true if there was a change
      */
    bool setGroupSpacingWidth( KPixelX groupSpacingWidth );
    /** sets width of digits and recalculates depend sizes
      * returns true if there was a change
      */
    bool setDigitWidth( KPixelX digitWidth );
    /** sets the metrics of the used font
      * @param digitWidth the new width of a digit
      * @param digitBaseLine the new baseline of the digits
      */
    void setMetrics( KPixelX digitWidth, KPixelY digitBaseLine, KPixelY digitHeight );
    /** */
    void set( AbstractByteArrayModel* byteArrayModel );
    /** creates new buffer for x-values; to be called on any change of NoOfBytesPerLine or metrics */
    void resetXBuffer();
    /** sets the codec to be used by the char column. */
    void setCharCodec( const CharCodec* charCodec );

    void setValueCodec( ValueCoding valueCoding, const ValueCodec* valueCodec );
    /** sets the spacing in the middle of a binary byte in the value column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    bool setBinaryGapWidth( KPixelX binaryGapWidth );

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
    KDE::Section linePositionsOfX( KPixelX x, KPixelX width ) const;
    /** returns byte pos at pixel with absolute x-coord x */
    int linePositionOfX( KPixelX x ) const;
    /** returns byte pos at pixel with absolute x-coord x, and sets the flag to true if we are closer to the right */
    int magneticLinePositionOfX( KPixelX x ) const;
    /** returns absolute x-coord of byte at position posInLine */
    KPixelX xOfLinePosition( int posInLine ) const;
    /** returns right absolute x-coord of byte at position posInLine */
    KPixelX rightXOfLinePosition( int posInLine ) const;
    /** returns byte pos at pixel with relative x-coord x */
    int linePositionOfColumnX( KPixelX x ) const;
    /** returns byte linePositions covered by pixels with relative x-coord x */
    KDE::Section linePositionsOfColumnXs( KPixelX x, KPixelX width ) const;
    /** returns relative x-coord of byte at position posInLine */
    KPixelX columnXOfLinePosition( int posInLine ) const;
    /** returns right relative x-coord of byte at position posInLine */
    KPixelX columnRightXOfLinePosition( int posInLine ) const;
    /** returns the linePositions that overlap with the x-coords relative to the view */
    KDE::Section visibleLinePositions( KPixelX x, KPixelX width ) const;
    /** returns the */
    KPixelXs xsOfLinePositionsInclSpaces( const KDE::Section& linePositions ) const;
    /** */
    KPixelXs columnXsOfLinePositionsInclSpaces( const KDE::Section& linePositions ) const;

    AbstractByteArrayView::CodingTypeId codingIdofY( KPixelY y ) const;
    KPixelY yOfCodingId( AbstractByteArrayView::CodingTypeId codingId ) const;

  public: // value access
    KPixelX byteWidth() const;
    KPixelX digitWidth() const;
    KPixelX groupSpacingWidth() const;
    KPixelX byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    KPixelY digitHeight() const;
    KPixelY rowHeight() const;

    KPixelX binaryGapWidth() const;

    /** returns true if "unprintable" chars (>32) are displayed in the char column
      * with their corresponding character, default is false
      */
    bool isShowingNonprinting() const;
    /** returns the actually used substitute character for "unprintable" chars, default is '.' */
    QChar substituteChar() const;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    QChar undefinedChar() const;

    int firstLinePos() const;
    int lastLinePos()  const;
    KDE::Section visibleLinePositions() const;
    const ByteArrayTableLayout *layout() const;
    bool isByteTypeColored() const;

    int visibleCodings() const;


  protected: // API to be redefined
    void renderByteText( QPainter* painter,
                         Byte byte, Character charByte, int codings, const QColor& color ) const;
    /** default implementation sets byte width to one digit width */
    void recalcByteWidth();


  protected:
    void renderPlain( QPainter* painter, const KDE::Section& linePositions, Address byteIndex );
    void renderSelection( QPainter* painter, const KDE::Section& linePositions, Address byteIndex, int flag );
    void renderMarking( QPainter* painter, const KDE::Section& linePositions, Address byteIndex, int flag );
    void renderRange( QPainter* painter, const QBrush& brush, const KDE::Section& linePositions, int flag );
    void renderBookmark( QPainter* painter, const QBrush& brush );

    void renderCode( QPainter* painter, const QString& code, const QColor& color ) const;

    void recalcX();

    bool isSelected( const AddressRange& range, AddressRange* selection, unsigned int* flag ) const;
    bool isMarked( const AddressRange& range, AddressRange* marking, unsigned int* flag ) const;

    void setByteWidth( int byteWidth );


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

    int mVisibleCodings;
    /** */
    KPixelX mDigitWidth;
    /** */
    KPixelY mDigitBaseLine;
    /** */
    KPixelY mDigitHeight;


  protected:  // individual data
    /** total width of byte display in pixel */
    KPixelX mByteWidth;
    /** width of inserting cursor in pixel */
//     KPixelX mCursorWidth;
    /** size of the line margin */
    KPixelX mByteSpacingWidth;
    /** width of spacing in pixel */
    KPixelX mGroupSpacingWidth;

    /** number of grouped bytes */
    int mNoOfGroupedBytes;

    /** pointer to array with buffered linePositions (relative to column position)
      * any spacing gets assigned to the byte left to it -> ...|c|c|c |c|c...
      */
    KPixelX* mLinePosLeftPixelX;
    KPixelX* mLinePosRightPixelX;
    /** index of right position */
    int mLastLinePos;

    /** */
    bool mByteTypeColored;

  protected: // value specifics
    /** */
    ValueCoding mValueCoding;
    /** */
    const ValueCodec* mValueCodec;
    /** */
    KPixelX mBinaryGapWidth;

  protected: // buffered data
    /** buffer to hold the formatted valueCoding */
    mutable QString mDecodedByteText;
    /** calculated: Offset in pixels of the second half of the binary */
    KPixelX mBinaryHalfOffset;

  protected: // char specifics
    /** */
    bool mShowingNonprinting;
    /** */
    QChar mSubstituteChar;
    /** */
    QChar mUndefinedChar;

  protected: // buffering rendering data
    KDE::Section mRenderLinePositions;
    int mRenderLine;
    KPixelX mRenderX;
    KPixelX mRenderWidth;
    int mSpacingTrigger;
};

inline int ByteArrayRowColumnRenderer::visibleCodings() const { return mVisibleCodings; }


inline KPixelX ByteArrayRowColumnRenderer::byteWidth()         const { return mByteWidth; }
inline KPixelX ByteArrayRowColumnRenderer::digitWidth()        const { return mDigitWidth; }
inline KPixelX ByteArrayRowColumnRenderer::byteSpacingWidth()  const { return mByteSpacingWidth; }
inline KPixelX ByteArrayRowColumnRenderer::groupSpacingWidth() const { return mGroupSpacingWidth; }
inline KPixelY ByteArrayRowColumnRenderer::digitHeight()       const { return mDigitHeight; }
inline int ByteArrayRowColumnRenderer::noOfGroupedBytes()      const { return mNoOfGroupedBytes; }

inline int ByteArrayRowColumnRenderer::firstLinePos() const { return mRenderLinePositions.start(); }
inline int ByteArrayRowColumnRenderer::lastLinePos()  const { return mRenderLinePositions.end(); }
inline KDE::Section ByteArrayRowColumnRenderer::visibleLinePositions() const { return mRenderLinePositions; }

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

inline void ByteArrayRowColumnRenderer::setByteWidth( int byteWidth ) { mByteWidth = byteWidth; }

inline KPixelX ByteArrayRowColumnRenderer::binaryGapWidth()                 const { return mBinaryGapWidth; }

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
