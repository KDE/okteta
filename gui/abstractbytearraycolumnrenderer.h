/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_ABSTRACTBYTEARRAYCOLUMNRENDERER_H
#define OKTETA_ABSTRACTBYTEARRAYCOLUMNRENDERER_H

// ColumnsView
#include "abstractcolumnrenderer.h"
#include "linepositionrange.h"
#include "linerange.h"
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
class OKTETAGUI_EXPORT AbstractByteArrayColumnRenderer : public AbstractColumnRenderer
{
  public:
    enum FrameStyle { Frame, Left, Right };
  public:
    AbstractByteArrayColumnRenderer( AbstractColumnStylist* stylist,
        AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges );
    virtual ~AbstractByteArrayColumnRenderer();


  public: // AbstractColumnRenderer API
    virtual void renderFirstLine( QPainter* painter, const PixelXRange& Xs, Line firstLineIndex );
    virtual void renderNextLine( QPainter* painter );

  public:
    void prepareRendering( const PixelXRange& Xs );

  public:
    //void renderLine( QPainter* painter, int lineIndex );
    void renderLinePositions( QPainter* painter, Line lineIndex, const LineRange& linePositions );
    /** paints a cursor based on the type of the byte.
      * @param byteIndex Index of the byte to paint the cursor for. If -1 a space is used as char.
      */
    void renderCursor( QPainter* painter, Address byteIndex );
    /** paints the byte with background.
      * @param byteIndex Index of the byte to paint. If -1 only the background is painted.
      */
    void renderByte( QPainter* painter, Address byteIndex );
    /** paints the byte with background and a frame around.
      * @param byteIndex Index of the byte to paint the frame for. If -1 a space is used as char.
      * @param style the style of the framing
      */
    void renderFramedByte( QPainter* painter, Address byteIndex, FrameStyle style );


  public: // modification access
    /** sets the spacing in the hex column
      * @param byteSpacingWidth spacing between the bytes in pixels
      * @param noOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * @param groupSpacingWidth spacing between the groups in pixels
      * returns true if there was a change
      */
    bool setSpacing( PixelX byteSpacingWidth, int noOfGroupedBytes = 0, PixelX groupSpacingWidth = 0 );
    /** sets the spacing between the bytes in the hex column
      * @param byteSpacingWidth spacing between the bytes in pixels
      * returns true if there was a change
      */
    bool setByteSpacingWidth( PixelX byteSpacingWidth );
    /** sets the number of grouped bytes in the hex column
      * @param noOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * returns true if there was a change
      */
    bool setNoOfGroupedBytes( int noOfGroupedBytes );
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

    void setByteTypeColored( bool byteTypeColored );


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
    virtual void renderByteText( QPainter* painter, Byte byte, Character charByte, const QColor& color ) const = 0;
    /** default implementation sets byte width to one digit width */
    virtual void recalcByteWidth();


  protected:
    void renderPlain( QPainter* painter, const LinePositionRange& linePositions, Address byteIndex );
    void renderSelection( QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag );
    void renderMarking( QPainter* painter, const LinePositionRange& linePositions, Address byteIndex, int flag );
    void renderRange( QPainter* painter, const QBrush& brush, const LinePositionRange& linePositions, int flag );
    void renderSelectionSpaceBehind( QPainter* painter, LinePosition linePosition );
    void renderSpaceBehind( QPainter* painter, const QBrush& brush, LinePosition linePosition );
    void renderBookmark( QPainter* painter, const QBrush& brush );

    void recalcX();

    bool getNextSelectedAddressRange( AddressRange* selectedRange, unsigned int* flag, const AddressRange& range ) const;
    bool getNextMarkedAddressRange( AddressRange* markedRange, unsigned int* flag, const AddressRange& range ) const;

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

    /** */
    PixelX mDigitWidth;
    /** */
    PixelY mDigitBaseLine;

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
    int mNoOfGroupedBytes;

    /** pointer to array with buffered linePositions (relative to column position)
      * a spacing gets assigned to the left byte -> ...c|c|c |c|c...
      */
    PixelX* mLinePosLeftPixelX;
    PixelX* mLinePosRightPixelX;
    /** index of right position */
    LinePosition mLastLinePos;

    /** */
    bool mByteTypeColored;


  protected: // buffering rendering data
    LinePositionRange mRenderLinePositions;
    Line mRenderLine;
    PixelX mRenderX;
    PixelX mRenderWidth;
    int mSpacingTrigger;
};


inline PixelX AbstractByteArrayColumnRenderer::byteWidth()         const { return mByteWidth; }
inline PixelX AbstractByteArrayColumnRenderer::digitWidth()        const { return mDigitWidth; }
inline PixelX AbstractByteArrayColumnRenderer::byteSpacingWidth()  const { return mByteSpacingWidth; }
inline PixelX AbstractByteArrayColumnRenderer::groupSpacingWidth() const { return mGroupSpacingWidth; }

inline int AbstractByteArrayColumnRenderer::noOfGroupedBytes()      const { return mNoOfGroupedBytes; }

inline LinePosition AbstractByteArrayColumnRenderer::firstLinePos() const { return mRenderLinePositions.start(); }
inline LinePosition AbstractByteArrayColumnRenderer::lastLinePos()  const { return mRenderLinePositions.end(); }
inline LinePositionRange AbstractByteArrayColumnRenderer::visibleLinePositions() const { return mRenderLinePositions; }

inline const ByteArrayTableLayout* AbstractByteArrayColumnRenderer::layout() const { return mLayout; }

inline void AbstractByteArrayColumnRenderer::setCharCodec( const CharCodec* charCodec )
{
    mCharCodec = charCodec;
}

inline void AbstractByteArrayColumnRenderer::setByteTypeColored( bool byteTypeColored )
{
    mByteTypeColored = byteTypeColored;
}
inline bool AbstractByteArrayColumnRenderer::isByteTypeColored() const { return mByteTypeColored; }

inline void AbstractByteArrayColumnRenderer::setByteWidth( int byteWidth ) { mByteWidth = byteWidth; }

}

#endif
