/***************************************************************************
                          kdatacolumn.h  -  description
                             -------------------
    begin                : Mit Mai 14 2003
    copyright            : (C) 2003,2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_UI_KDATACOLUMN_H
#define KHE_UI_KDATACOLUMN_H

// lib
#include "kdatalayout.h"
// commonlib
#include <ksection.h>
//
#include <kcolumn.h>
// Okteta core
#include <khe.h>
#include <kabstractbytearraymodel.h>
#include <khechar.h>


class QPainter;
class QColor;
class QBrush;

namespace KHECore {
class KCharCodec;
}

namespace KHEUI
{

// class KByteArrayView;
class KDataRanges;

const int NoByteFound = -1;

/** base class of all buffer column displayers
  * holds all information about the vertical layout of a buffer column
  * knows how to paint the data and the editing things (focus, cursor, selection)
  * but does not offer
  *
  *@author Friedrich W. H. Kossebauint KDataColumn::posOfX( KPixelX PX, bool *ToTheRightFlag ) const
  */
class KDataColumn : public KColumn
{
  public:
    enum KFrameStyle { Frame, Left, Right };
  public:
    KDataColumn( KColumnsView/*KByteArrayView*/ *HE, KHECore::KAbstractByteArrayModel *B, KDataLayout *L, KDataRanges *R );
    virtual ~KDataColumn();


  public: // KColumn-API
    virtual void paintFirstLine( QPainter *P, const KPixelXs &Xs, int FirstLine );
    virtual void paintNextLine( QPainter *P );

  public:
    void preparePainting( const KPixelXs &Xs );

  public:
    //void paintLine( QPainter *P, int Line );
    void paintPositions( QPainter *P, int Line, const KHE::KSection &Positions );
    /** paints a cursor based on the type of the byte.
      * @param Index Index of the byte to paint the cursor for. If -1 a space is used as char.
      */
    void paintCursor( QPainter *P, int Index );
    /** paints the byte with background.
      * @param Index Index of the byte to paint. If -1 only the background is painted.
      */
    void paintByte( QPainter *P, int Index );
    /** paints the byte with background and a frame around.
      * @param Index Index of the byte to paint the frame for. If -1 a space is used as char.
      * @param Style the style of the framing
      */
    void paintFramedByte( QPainter *P, int Index, KFrameStyle Style );


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
    void set( KHECore::KAbstractByteArrayModel *B );
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
    const KDataLayout *layout() const;
    KHECore::KCharCodec* codec() const;


  protected: // API to be redefined
    /** default implementation simply prints the byte as ASCII */
    virtual void drawByte( QPainter *P, char Byte, KHECore::KChar B, const QColor &Color ) const;
    /** default implementation sets byte width to one digit width */
    virtual void recalcByteWidth();


  protected:
    void paintPlain( QPainter *P, const KHE::KSection &Positions, int Index );
    void paintSelection( QPainter *P, const KHE::KSection &Positions, int Index, int Flag );
    void paintMarking( QPainter *P, const KHE::KSection &Positions, int Index, int Flag );
    void paintRange( QPainter *P, const QBrush &Brush, const KHE::KSection &Positions, int Flag );

    void recalcX();

    bool isSelected( const KHE::KSection &Range, KHE::KSection *Selection, unsigned int *Flag ) const;
    bool isMarked( const KHE::KSection &Range, KHE::KSection *Marking, unsigned int *Flag ) const;


  protected:
    /** pointer to the buffer */
    KHECore::KAbstractByteArrayModel *Buffer;
    /** pointer to the layout */
    const KDataLayout *Layout;
    /** pointer to the ranges */
    KDataRanges *Ranges;
    /** */
    KHECore::KCharCodec *Codec;

    /** */
    KPixelX DigitWidth;
    /** */
    KPixelY DigitBaseLine;


  protected:  // individual data
    /** total width of byte display in pixel */
    KPixelX ByteWidth;
    /** width of inserting cursor in pixel */
    KPixelX CursorWidth;
    /** size of the line margin */
    KPixelX ByteSpacingWidth;
    /** width of spacing in pixel */
    KPixelX GroupSpacingWidth;

    /** number of grouped bytes */
    int NoOfGroupedBytes;

    /** pointer to array with buffered positions (relative to column position)
      * a spacing gets assigned to the left byte -> ...c|c|c |c|c...
      */
    KPixelX *PosX;
    KPixelX *PosRightX;
    /** index of right position */
    int LastPos;


  protected: // buffering drawing data
    KHE::KSection PaintPositions;
    int PaintLine;
    KPixelX PaintX;
    KPixelX PaintW;
    int SpacingTrigger;
};


inline KPixelX KDataColumn::byteWidth()         const { return ByteWidth; }
inline KPixelX KDataColumn::digitWidth()        const { return DigitWidth; }
inline KPixelX KDataColumn::byteSpacingWidth()  const { return ByteSpacingWidth; }
inline KPixelX KDataColumn::groupSpacingWidth() const { return GroupSpacingWidth; }

inline int KDataColumn::noOfGroupedBytes()      const { return NoOfGroupedBytes; }

inline int KDataColumn::firstPos() const { return PaintPositions.start(); }
inline int KDataColumn::lastPos()  const { return PaintPositions.end(); }
inline KHE::KSection KDataColumn::visiblePositions() const { return PaintPositions; }

inline const KDataLayout *KDataColumn::layout() const { return Layout; }

inline void KDataColumn::setCodec( KHECore::KCharCodec *C ) { Codec = C; }
inline KHECore::KCharCodec* KDataColumn::codec() const { return Codec; }

}

#endif
