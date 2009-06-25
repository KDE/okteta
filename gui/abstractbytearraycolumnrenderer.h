/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2007,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KHE_UI_ABSTRACTBYTEARRAYCOLUMNRENDERER_H
#define KHE_UI_ABSTRACTBYTEARRAYCOLUMNRENDERER_H

// ColumnsView
#include <abstractcolumnrenderer.h>
// Okteta core
#include <khe.h>
#include <abstractbytearraymodel.h>
#include <khechar.h>
// commonlib
#include <section.h>


class QPainter;
class QColor;
class QBrush;

namespace KHECore {
class Bookmarkable;
class CharCodec;
}

namespace KHEUI
{

// class KByteArrayView;
class ByteArrayTableRanges;
class ByteArrayTableLayout;

const int NoByteFound = -1;

/** base class of all buffer column displayers
  * holds all information about the vertical layout of a buffer column
  * knows how to paint the data and the editing things (focus, cursor, selection)
  * but does not offer
  *
  *@author Friedrich W. H. Kossebau
  */
class AbstractByteArrayColumnRenderer : public AbstractColumnRenderer
{
  public:
    enum FrameStyle { Frame, Left, Right };
  public:
    AbstractByteArrayColumnRenderer( ColumnsView/*KByteArrayView*/ *columnsView,
        KHECore::AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges );
    virtual ~AbstractByteArrayColumnRenderer();


  public: // AbstractColumnRenderer API
    virtual void renderFirstLine( QPainter *painter, const KPixelXs &Xs, int firstLineIndex );
    virtual void renderNextLine( QPainter *painter );

  public:
    void prepareRendering( const KPixelXs &Xs );

  public:
    //void renderLine( QPainter *painter, int lineIndex );
    void renderLinePositions( QPainter *painter, int lineIndex, const KHE::Section &linePositions );
    /** paints a cursor based on the type of the byte.
      * @param byteIndex Index of the byte to paint the cursor for. If -1 a space is used as char.
      */
    void renderCursor( QPainter *painter, int byteIndex );
    /** paints the byte with background.
      * @param byteIndex Index of the byte to paint. If -1 only the background is painted.
      */
    void renderByte( QPainter *painter, int byteIndex );
    /** paints the byte with background and a frame around.
      * @param byteIndex Index of the byte to paint the frame for. If -1 a space is used as char.
      * @param Style the style of the framing
      */
    void renderFramedByte( QPainter *painter, int byteIndex, FrameStyle style );


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
    void setMetrics( KPixelX digitWidth, KPixelY digitBaseLine );
    /** */
    void set( KHECore::AbstractByteArrayModel *byteArrayModel );
    /** creates new buffer for x-values; to be called on any change of NoOfBytesPerLine or metrics */
    void resetXBuffer();
    /** sets the codec to be used by the char column. */
    void setCharCodec( const KHECore::CharCodec* charCodec );

    void setByteTypeColored( bool byteTypeColored );


  public: // functional logic
    /** returns byte linePositions covered by pixels with absolute x-coord x */
    KHE::Section linePositionsOfX( KPixelX x, KPixelX width ) const;
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
    KHE::Section linePositionsOfColumnXs( KPixelX x, KPixelX width ) const;
    /** returns relative x-coord of byte at position posInLine */
    KPixelX columnXOfLinePosition( int posInLine ) const;
    /** returns right relative x-coord of byte at position posInLine */
    KPixelX columnRightXOfLinePosition( int posInLine ) const;
    /** returns the linePositions that overlap with the x-coords relative to the view */
    KHE::Section visibleLinePositions( KPixelX x, KPixelX width ) const;
    /** returns the */
    KPixelXs xsOfLinePositionsInclSpaces( const KHE::Section &linePositions ) const;
    /** */
    KPixelXs columnXsOfLinePositionsInclSpaces( const KHE::Section &linePositions ) const;

  public: // value access
    KPixelX byteWidth() const;
    KPixelX digitWidth() const;
    KPixelX groupSpacingWidth() const;
    KPixelX byteSpacingWidth() const;
    int noOfGroupedBytes() const;

    int firstLinePos() const;
    int lastLinePos()  const;
    KHE::Section visibleLinePositions() const;
    const ByteArrayTableLayout *layout() const;
    bool isByteTypeColored() const;


  protected: // API to be redefined
    virtual void renderByteText( QPainter *painter, char byte, KHECore::KChar charByte, const QColor &color ) const = 0;
    /** default implementation sets byte width to one digit width */
    virtual void recalcByteWidth();


  protected:
    void renderPlain( QPainter *painter, const KHE::Section &linePositions, int byteIndex );
    void renderSelection( QPainter *painter, const KHE::Section &linePositions, int byteIndex, int flag );
    void renderMarking( QPainter *painter, const KHE::Section &linePositions, int byteIndex, int flag );
    void renderRange( QPainter *painter, const QBrush &brush, const KHE::Section &linePositions, int flag );
    void renderBookmark( QPainter *painter, const QBrush &brush );

    void recalcX();

    bool isSelected( const KHE::Section &range, KHE::Section *selection, unsigned int *flag ) const;
    bool isMarked( const KHE::Section &range, KHE::Section *marking, unsigned int *flag ) const;

    void setByteWidth( int byteWidth );


  protected:
    /** pointer to the buffer */
    KHECore::AbstractByteArrayModel *mByteArrayModel;
    /** pointer to the layout */
    const ByteArrayTableLayout *mLayout;
    /** pointer to the ranges */
    ByteArrayTableRanges* mRanges;
    /** */
    KHECore::Bookmarkable *mBookmarks;
    /** */
    const KHECore::CharCodec* mCharCodec;

    /** */
    KPixelX mDigitWidth;
    /** */
    KPixelY mDigitBaseLine;


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
      * a spacing gets assigned to the left byte -> ...c|c|c |c|c...
      */
    KPixelX *mLinePosLeftPixelX;
    KPixelX *mLinePosRightPixelX;
    /** index of right position */
    int mLastLinePos;

    /** */
    bool mByteTypeColored;


  protected: // buffering rendering data
    KHE::Section mRenderLinePositions;
    int mRenderLine;
    KPixelX mRenderX;
    KPixelX mRenderWidth;
    int mSpacingTrigger;
};


inline KPixelX AbstractByteArrayColumnRenderer::byteWidth()         const { return mByteWidth; }
inline KPixelX AbstractByteArrayColumnRenderer::digitWidth()        const { return mDigitWidth; }
inline KPixelX AbstractByteArrayColumnRenderer::byteSpacingWidth()  const { return mByteSpacingWidth; }
inline KPixelX AbstractByteArrayColumnRenderer::groupSpacingWidth() const { return mGroupSpacingWidth; }

inline int AbstractByteArrayColumnRenderer::noOfGroupedBytes()      const { return mNoOfGroupedBytes; }

inline int AbstractByteArrayColumnRenderer::firstLinePos() const { return mRenderLinePositions.start(); }
inline int AbstractByteArrayColumnRenderer::lastLinePos()  const { return mRenderLinePositions.end(); }
inline KHE::Section AbstractByteArrayColumnRenderer::visibleLinePositions() const { return mRenderLinePositions; }

inline const ByteArrayTableLayout *AbstractByteArrayColumnRenderer::layout() const { return mLayout; }

inline void AbstractByteArrayColumnRenderer::setCharCodec( const KHECore::CharCodec* charCodec )
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
