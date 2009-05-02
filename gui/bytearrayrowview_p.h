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

#ifndef KHE_UI_BYTEARRAYROWVIEW_P_H
#define KHE_UI_BYTEARRAYROWVIEW_P_H

// lib
#include "bytearrayrowview.h"
#include "abstractbytearrayview_p.h"
#include "bytearrayrowcolumnrenderer.h"
#include "offsetcolumnrenderer.h"

class QTimer;


namespace KHEUI
{
class BorderColumnRenderer;

class KCursor;

class Coord;


class ByteArrayRowViewPrivate : public AbstractByteArrayViewPrivate
{
    Q_DECLARE_PUBLIC( ByteArrayRowView )

  public:
    explicit ByteArrayRowViewPrivate( ByteArrayRowView* parent );
    virtual ~ByteArrayRowViewPrivate();

  public:
    void init();

  public:
    bool offsetColumnVisible() const;

  // value column
    int/*KPixelX*/ byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    int/*KPixelX*/ groupSpacingWidth() const;
    int/*KPixelX*/ binaryGapWidth() const;

  // char column
    bool showsNonprinting() const;
    QChar substituteChar() const;
    QChar undefinedChar() const;

    bool isByteTypeColored() const;

  public: // logic value service
    QSize minimumSizeHint() const;

  public: // modification access
    void setByteArrayModel( KHECore::AbstractByteArrayModel* byteArrayModel );

  public: // AbstractByteArrayViewPrivate API
    virtual void ensureCursorVisible();
    virtual void placeCursor( const QPoint& point );
    virtual QRect cursorRect() const;

  public:
    void toggleOffsetColumn( bool offsetColumnVisible );

  // setting parameters
  // value column parameters
    void setByteSpacingWidth( int/*KPixelX*/ byteSpacingWidth ) ;
    void setNoOfGroupedBytes( int noOfGroupedBytes );
    void setGroupSpacingWidth( int/*KPixelX*/ groupSpacingWidth );
    void setBinaryGapWidth( int binaryGapWidth );
    void setBufferSpacing( /*KPixelX*/int byteSpacingWidth, int noOfGroupedBytes, /*KPixelX*/int groupSpacingWidth );
    void setValueCoding( AbstractByteArrayView::ValueCoding valueCoding );
  // char column parameters
    void setShowsNonprinting( bool showsNonprinting = true );
    void setSubstituteChar( QChar substituteChar );
    void setUndefinedChar( QChar undefinedChar );
    void setCharCoding( AbstractByteArrayView::CharCoding charCoding );
    void setCharCoding( const QString& charCodingName );
    void setByteTypeColored( bool isColored );

  public: // events
    void mousePressEvent( QMouseEvent* mousePressEvent );
    void mouseReleaseEvent( QMouseEvent* mouseReleaseEvent );
    void mouseMoveEvent( QMouseEvent* mouseMoveEvent );
    void mouseDoubleClickEvent( QMouseEvent* mouseDoubleClickEvent );

  public: // slots
    /** gets called by the cursor blink timer */
    void blinkCursor();
    /** gets called by the scroll timer (for mouse selection) */
    void autoScrollTimerDone();
    /** */
    void clipboardChanged();
    /** */
    void startDrag();

  protected: // drawing related operations
    /** recreates the cursor pixmaps and paints active and inactive cursors if doable */
    void updateCursors();
    void createCursorPixmaps();
    /** draws the blinking cursor or removes it */
    void drawActiveCursor( QPainter* painter );
    void drawInactiveCursor( QPainter* painter );
    void updateCursor( const ByteArrayRowColumnRenderer& column );

  protected: // partial operations
    void handleMouseMove( const QPoint& point );
    /** sets ChangedRange to the range of VisibleRange that is actually changed
      * @return true if there was a change within the visible range
      */
    void handleInternalDrag( QDropEvent* dropEvent );
    void handleFontChange( const QFont& oldFont );

  protected:
    /** recalcs a layout due to the resize style that fits into the view size
      * and updates the dependant values
      */
    void ensureVisible( const ByteArrayRowColumnRenderer& column, const Coord& coord );
    void renderColumns( QPainter* painter, int cx, int cy, int cw, int ch );

  protected: // AbstractByteArrayViewPrivate API
  // cursor control
    virtual void startCursor();
    virtual void stopCursor();
    virtual void pauseCursor();
    virtual void unpauseCursor();

    virtual void setActiveCoding( AbstractByteArrayView::CodingTypeId codingId );
    virtual void setVisibleCodings( int visibleCodings );

    virtual int indexByPoint( const QPoint& point ) const;

  protected: // AbstractByteArrayViewPrivate API
    virtual AbstractByteArrayView::CodingTypeId activeCoding() const;
    virtual int visibleCodings() const;
    virtual int fittingBytesPerLine() const;
    virtual void adjustToLayoutNoOfBytesPerLine();
    virtual void updateChanged();

  protected:
    OffsetColumnRenderer*         mOffsetColumn;
    BorderColumnRenderer*         mBorderColumn;
    ByteArrayRowColumnRenderer*   mByteArrayColumn;

    ByteArrayRowView::CodingTypeId mActiveCoding;
    ByteArrayRowView::CodingTypeId mInactiveCoding;

  protected:
    /** Timer that controls the blinking of the cursor */
    QTimer* mCursorBlinkTimer;
    /** Timer that triggers ensureCursorVisible function calls */
    QTimer* mScrollTimer;
/*     QTimer *ChangeIntervalTimer, */
    /** Timer to start a drag */
    QTimer* mDragStartTimer;
    /** timer to measure whether the time between a double click and the following counts for a tripleclick */
    QTimer* mTrippleClickTimer;

    /** object to store the blinking cursor pixmaps */
    KCursor* mCursorPixmaps;

  protected:
    /** point at which the current double click happended (used by TrippleClick) */
    QPoint mDoubleClickPoint;
    /** line in which the current double click happended (used by TrippleClick) */
    int mDoubleClickLine;
    /** point at which the current dragging started */
    QPoint mDragStartPoint;

  protected: // parameters
    /** flag if a mouse button is pressed */
    bool mMousePressed:1;
    /** flag if a double click is happening */
    bool mInDoubleClick:1;
    /** flag if a Drag'n'Drop is happening */
    bool mInDnD:1;
    /** flag if a drag might have started */
    bool mDragStartPossible:1;
    /** flag if the cursor is visible */
    bool mBlinkCursorVisible:1;
};

inline KPixelX ByteArrayRowViewPrivate::byteSpacingWidth()  const { return mByteArrayColumn->byteSpacingWidth(); }
inline int ByteArrayRowViewPrivate::noOfGroupedBytes()      const { return mByteArrayColumn->noOfGroupedBytes(); }
inline KPixelX ByteArrayRowViewPrivate::groupSpacingWidth() const { return mByteArrayColumn->groupSpacingWidth(); }
inline KPixelX ByteArrayRowViewPrivate::binaryGapWidth()    const { return mByteArrayColumn->binaryGapWidth(); }
inline bool ByteArrayRowViewPrivate::showsNonprinting()     const { return mByteArrayColumn->isShowingNonprinting(); }
inline QChar ByteArrayRowViewPrivate::substituteChar()      const { return mByteArrayColumn->substituteChar(); }
inline QChar ByteArrayRowViewPrivate::undefinedChar()       const { return mByteArrayColumn->undefinedChar(); }
inline bool ByteArrayRowViewPrivate::isByteTypeColored()    const { return mByteArrayColumn->isByteTypeColored(); }
inline bool ByteArrayRowViewPrivate::offsetColumnVisible()  const { return mOffsetColumn->isVisible(); }

}

#endif
