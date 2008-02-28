/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2003,2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTECOLUMNRENDERER_H
#define BYTECOLUMNRENDERER_H

// lib
#include "abstractcolumnrenderer.h"
// Okteta gui
#include <kdatalayout.h>
// Okteta core
#include <khe.h>
#include <kabstractbytearraymodel.h>
#include <khechar.h>
#include <ksection.h>


class QPainter;
class QColor;
class QBrush;

class AbstractColumnFrameRenderer;

namespace KDE { namespace If {
class Bookmarks;
} }
namespace KHECore {
class KCharCodec;
}
const int NoByteFound = -1;

/** base class of all buffer column displayers
  * holds all information about the vertical layout of a buffer column
  * knows how to paint the data and the editing things (focus, cursor, selection)
  * but does not offer
  *
  *@author Friedrich W. H. Kossebau
  */
class ByteColumnRenderer : public AbstractColumnRenderer
{
  public:
    enum KFrameStyle { Frame, Left, Right };
  public:
    ByteColumnRenderer( AbstractColumnFrameRenderer *columnFrameRenderer,
                        const KHECore::KAbstractByteArrayModel *byteArray,
                        const KHEUI::KDataLayout *layout );
    virtual ~ByteColumnRenderer();


  public: // AbstractColumnRenderer API
    virtual void renderFirstLine( QPainter *painter, const KPixelXs &Xs, int firstLine );
    virtual void renderNextLine( QPainter *painter );

  public:
    void prepareRendering( const KPixelXs &Xs );

  public:
    void renderPositions( QPainter *painter, int Line, const KHE::KSection &Positions );

  public: // modification access
    /** sets the spacing in the hex column
      * @param ByteSpacingW spacing between the bytes in pixels
      * @param NewNoOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * @param GroupSpacingW spacing between the groups in pixels
      * returns true if there was a change
      */
    bool setSpacing( KPixelX ByteSpacingW, int NewNoOfGroupedBytes = 0, KPixelX GroupSpacingW = 0 );
    /** sets the spacing between the bytes in the hex column
      * @param ByteSpacingW spacing between the bytes in pixels
      * returns true if there was a change
      */
    bool setByteSpacingWidth( KPixelX ByteSpacingW );
    /** sets the number of grouped bytes in the hex column
      * @param NewNoOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * returns true if there was a change
      */
    bool setNoOfGroupedBytes( int NewNoOfGroupedBytes );
    /** sets the spacing between the groups of bytes in the hex column
      * @param GroupSpacingW spacing between the groups in pixels
      * returns true if there was a change
      */
    bool setGroupSpacingWidth( KPixelX GroupSpacingW );
    /** sets width of digits and recalculates depend sizes
      * returns true if there was a change
      */
    bool setDigitWidth( KPixelX DW );
    /** sets the metrics of the used font
      * @param NewDigitWidth the new width of a digit
      * @param NewDigitBaseLine the new baseline of the digits
      */
    void setMetrics( KPixelX NewDigitWidth, KPixelY NewDigitBaseLine );
    /** */
    void setByteArrayModel( const KHECore::KAbstractByteArrayModel *byteArrayModel );
    /** creates new buffer for x-values; to be called on any change of NoOfBytesPerLine or metrics */
    void resetXBuffer();
    /** sets the codec to be used by the char column. */
    void setCodec( KHECore::KCharCodec *C );

  public: // functional logic
    /** returns byte positions covered by pixels with absolute x-coord x */
    KHE::KSection posOfX( KPixelX x, KPixelX w ) const;
    /** returns byte pos at pixel with absolute x-coord x */
    int posOfX( KPixelX x ) const;
    /** returns byte pos at pixel with absolute x-coord x, and sets the flag to true if we are closer to the right */
    int magPosOfX( KPixelX PX ) const;
    /** returns absolute x-coord of byte at position Pos */
    KPixelX xOfPos( int Pos ) const;
    /** returns right absolute x-coord of byte at position Pos */
    KPixelX rightXOfPos( int Pos ) const;
    /** returns byte pos at pixel with relative x-coord x */
    int posOfRelX( KPixelX x ) const;
    /** returns byte positions covered by pixels with relative x-coord x */
    KHE::KSection posOfRelX( KPixelX x, KPixelX w ) const;
    /** returns relative x-coord of byte at position Pos */
    KPixelX relXOfPos( int Pos ) const;
    /** returns right relative x-coord of byte at position Pos */
    KPixelX relRightXOfPos( int Pos ) const;
    /** returns the positions that overlap with the absolute x-coords */
    KHE::KSection visiblePositions( KPixelX x, KPixelX w ) const;
    /** returns the */
    KPixelXs wideXPixelsOfPos( const KHE::KSection &Positions ) const;
    /** */
    KPixelXs relWideXPixelsOfPos( const KHE::KSection &Positions ) const;

  public: // value access
    KPixelX byteWidth()                      const;
    KPixelX digitWidth()                     const;
    KPixelX groupSpacingWidth()              const;
    KPixelX byteSpacingWidth()               const;
    int noOfGroupedBytes()                   const;

    int firstPos() const;
    int lastPos()  const;
    KHE::KSection visiblePositions() const;
    const KHEUI::KDataLayout *layout() const;
    KHECore::KCharCodec* codec() const;


  protected: // API to be redefined
    /** default implementation simply prints the byte as ASCII */
    virtual void drawByte( QPainter *painter, char Byte, KHECore::KChar B, const QColor &Color ) const;
    /** default implementation sets byte width to one digit width */
    virtual void recalcByteWidth();


  protected:
    void renderPlain( QPainter *painter, const KHE::KSection &Positions, int Index );
    void renderRange( QPainter *painter, const QBrush &Brush, const KHE::KSection &Positions, int Flag );

    void recalcX();

  protected:
    /** pointer to the buffer */
    const KHECore::KAbstractByteArrayModel *mByteArrayModel;
    /** pointer to the layout */
    const KHEUI::KDataLayout *mLayout;
    /** */
//     KDE::If::Bookmarks *Bookmarks;
    /** */
    KHECore::KCharCodec *mCodec;

    /** */
    KPixelX mDigitWidth;
    /** */
    KPixelY mDigitBaseLine;


  protected:  // individual data
    /** total width of byte display in pixel */
    KPixelX mByteWidth;
    /** size of the line margin */
    KPixelX mByteSpacingWidth;
    /** width of spacing in pixel */
    KPixelX mGroupSpacingWidth;

    /** number of grouped bytes */
    int mNoOfGroupedBytes;

    /** pointer to array with buffered positions (relative to column position)
      * a spacing gets assigned to the left byte -> ...c|c|c |c|c...
      */
    KPixelX *mPosX;
    KPixelX *mPosRightX;
    /** index of right position */
    int mLastPos;


  protected: // buffering drawing data
    KHE::KSection mRenderPositions;
    int mRenderLine;
    KPixelX mRenderX;
    KPixelX mRenderW;
    int mSpacingTrigger;
};


inline KPixelX ByteColumnRenderer::byteWidth()         const { return mByteWidth; }
inline KPixelX ByteColumnRenderer::digitWidth()        const { return mDigitWidth; }
inline KPixelX ByteColumnRenderer::byteSpacingWidth()  const { return mByteSpacingWidth; }
inline KPixelX ByteColumnRenderer::groupSpacingWidth() const { return mGroupSpacingWidth; }

inline int ByteColumnRenderer::noOfGroupedBytes()      const { return mNoOfGroupedBytes; }

inline int ByteColumnRenderer::firstPos() const { return mRenderPositions.start(); }
inline int ByteColumnRenderer::lastPos()  const { return mRenderPositions.end(); }
inline KHE::KSection ByteColumnRenderer::visiblePositions() const { return mRenderPositions; }

inline const KHEUI::KDataLayout *ByteColumnRenderer::layout() const { return mLayout; }

inline void ByteColumnRenderer::setCodec( KHECore::KCharCodec *C ) { mCodec = C; }
inline KHECore::KCharCodec* ByteColumnRenderer::codec() const { return mCodec; }

#endif
