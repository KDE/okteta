/*
    This file is part of the Okteta Gui library, part of the KDE project.

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

#ifndef KHE_UI_KBYTEARRAYVIEW_H
#define KHE_UI_KBYTEARRAYVIEW_H

// lib
#include "columnsview.h"
// Okteta core
// #include "khe.h"
#include <ksection.h>
#include <ksectionlist.h>
// Qt
#include <QtGui/QClipboard>
#include <QtCore/QList>

class QTimer;

namespace KHECore {
class KBookmark;
class KCharCodec;
class KAbstractByteArrayModel;
}
namespace KHE {
class ArrayChangeMetricsList;
}

namespace KHEUI
{

class CoordRange;

class CharByteArrayColumnRenderer;
class ValueByteArrayColumnRenderer;
class AbstractByteArrayColumnRenderer;
class OffsetColumnRenderer;
class BorderColumnRenderer;

class Coord;
class KDataCursor;
class ByteArrayTableLayout;
class KDataRanges;

class KController;
class KTabController;
class KNavigator;
class KValueEditor;
class KCharEditor;

class KCursor;


/** the main widget
  * 
  * The functions split up in helper functions and those that are complete.
  * 
  * Complete functions can be called from the outside and leave the widget in 
  * a consistent state. They care for exceptions so one can safely call them in all 
  * situations (like empty buffer, cursor behind end etc.)
  *
  * Helper functions do only partial tasks and need to be completed. They often do not 
  * check for exceptions so one has to care for this.
  *
  *@author Friedrich W. H. Kossebau
  */

class OKTETAGUI_EXPORT KByteArrayView : public ColumnsView
{
  friend class KTabController;
  friend class KNavigator;
  friend class KEditor;
  friend class KValueEditor;
  friend class KCharEditor;

  Q_OBJECT
  Q_ENUMS( KResizeStyle KCoding )
  Q_PROPERTY( bool OverwriteMode READ isOverwriteMode WRITE setOverwriteMode )
  Q_PROPERTY( bool OverwriteOnly READ isOverwriteOnly WRITE setOverwriteOnly )
  Q_PROPERTY( bool Modified READ isModified WRITE setModified DESIGNABLE false )
  Q_PROPERTY( bool ReadOnly READ isReadOnly WRITE setReadOnly )

  Q_PROPERTY( int NoOfBytesPerLine READ noOfBytesPerLine WRITE setNoOfBytesPerLine )
  Q_PROPERTY( bool TabChangesFocus READ tabChangesFocus WRITE setTabChangesFocus )

  //Q_PROPERTY( bool hasSelectedData READ hasSelectedData )
  //Q_PROPERTY( QByteArray SelectedData READ selectedData )
  Q_PROPERTY( KResizeStyle ResizeStyle READ resizeStyle WRITE setResizeStyle )
  Q_PROPERTY( int StartOffset READ startOffset WRITE setStartOffset )
  Q_PROPERTY( int FirstLineOffset READ firstLineOffset WRITE setFirstLineOffset )
  Q_PROPERTY( int VisibleBufferColumns READ visibleBufferColumns WRITE showBufferColumns )
  // value column
  Q_PROPERTY( KCoding Coding READ coding WRITE setCoding )
  Q_PROPERTY( int ByteSpacingWidth READ byteSpacingWidth WRITE setByteSpacingWidth )
  Q_PROPERTY( int NoOfGroupedBytes READ noOfGroupedBytes WRITE setNoOfGroupedBytes )
  Q_PROPERTY( int GroupSpacingWidth READ groupSpacingWidth WRITE setGroupSpacingWidth )
  Q_PROPERTY( int BinaryGapWidth READ binaryGapWidth WRITE setBinaryGapWidth )
  // char column
  Q_PROPERTY( bool ShowNonprinting READ showsNonprinting WRITE setShowsNonprinting )
  Q_PROPERTY( QChar SubstituteChar READ substituteChar WRITE setSubstituteChar )

  public:
    enum KResizeStyle { NoResize=0, LockGrouping=1, FullSizeUsage=2, MaxResizeStyleId=0xFF };
    enum KCoding { HexadecimalCoding=0, DecimalCoding=1, OctalCoding=2, BinaryCoding=3, MaxCodingId=0xFFFF };
    enum KEncoding { LocalEncoding=0, ISO8859_1Encoding=1, EBCDIC1047Encoding=2,
                     StartOfOwnEncoding=0x8000, MaxEncodingId=0xFFFF };

    enum ByteArrayColumnId { ValueColumnId=1, CharColumnId=2 };


  public:
    explicit KByteArrayView( KHECore::KAbstractByteArrayModel *Model = 0, QWidget *Parent = 0 );
    virtual ~KByteArrayView();


  public: // ColumnsView API
    virtual void renderColumns( QPainter *p, int cx, int cy, int cw, int ch );

  public: // QWidget API
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;


  public: // value access
    KHECore::KAbstractByteArrayModel *byteArrayModel() const;
    /** returns true if there is a selected range in the array */
    bool hasSelectedData() const;
    /** returns the index of the cursor position */
    int cursorPosition() const;
    /***/
    bool isCursorBehind() const;
    ByteArrayColumnId cursorColumn() const;
    KHE::KSection selection() const;

    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;
    bool isReadOnly() const;
    bool isModified() const;

    bool tabChangesFocus() const;

    KResizeStyle resizeStyle() const;
    int noOfBytesPerLine() const;
    int startOffset() const;
    int firstLineOffset() const;

    bool offsetColumnVisible() const;
    int visibleBufferColumns() const;

  // value column
    KCoding coding() const;
    int/*KPixelX*/ byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    int/*KPixelX*/ groupSpacingWidth() const;
    int/*KPixelX*/ binaryGapWidth() const;

  // char column
    /** reports if "non-printing" chars are displayed in the char column
      * with their original character. Default is false
      * @return @c true if original chars are displayed, otherwise @c false 
      */
    bool showsNonprinting() const;
    /** gives the used substitute character for "unprintable" chars, default is '.'
      * @return substitute character 
      */
    QChar substituteChar() const;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    QChar undefinedChar() const;
    /**
      * @return encoding used in the char column
      */
    KEncoding encoding() const;
    /**
     * @return name of the encoding used in the char column
     */
    const QString &encodingName() const;

    double zoomLevel() const;

  public: // logic value service
    /** calculates the number of bytes per line that fit into a widget with the given size
      * tests whether a vertical scroll bar is needed at all or not due to the given width
      * takes the frame width into account
      * @param TestSize Size the widget might have
      * @return number of bytes per line that fit into a widget with the given size
      */
    int fittingBytesPerLine() const;
    /** detects the index of the byte at the given point
      * @param Point in viewport coordinate system
      * @return index of the byte that covers the point
      */
    int indexByPoint(const QPoint &Point ) const;

  public:

  public: // modification access
    /** puts the cursor to the position of index, handles all drawing 
      * @param Index 
      */
    void setCursorPosition( int Index, bool Behind=false );
    /** puts the cursor in the column at the pos of Point (in absolute coord), does not handle the drawing */
    void placeCursor( const QPoint &Point );
    /***/
    void setCursorColumn( ByteArrayColumnId );
//    void repaintByte( int row, int column, bool Erase = true );
//    void updateByte( int row, int column );
//    void ensureByteVisible( int row, int column );

    void setSelection( int Start, int End );

    void pasteData( const QMimeData *data );

  public:
    bool canReadData( const QMimeData *data ) const;
    /** 
      * @return deep copy of the selected data 
      */
    QByteArray selectedData() const;
    QMimeData *selectionAsMimeData() const;

  public Q_SLOTS:
    /** */
    void setByteArrayModel( KHECore::KAbstractByteArrayModel *B );

    /** switches the Offset column on/off */
    void toggleOffsetColumn( bool Visible );
    /** */
    void showBufferColumns( int Columns );
    /** scrolls the view as much as needed to have the cursor fully visible */
    void ensureCursorVisible();

  // setting parameters
    /** sets the resizestyle for the value column. Default is KHE::FullSizeUsage */
    void setResizeStyle( KResizeStyle Style );
    /** sets whether the widget is readonly or not, Default is true.
      * If the databuffer which is worked on can't be written the widget stays readonly
      */
    virtual void setReadOnly( bool b );
    /** sets whether the widget is overwriteonly or not. Default is false. */
    virtual void setOverwriteOnly( bool b );
    /** sets whether the widget is in overwrite mode or not. Default is true. */
    virtual void setOverwriteMode( bool b );
    /** sets whether the data should be treated modified or not */
    virtual void setModified( bool b );
    /** sets whether on a tab key there should be switched from the char column back to the value column
      * or be switched to the next focusable widget. Default is false
      */
    virtual void setTabChangesFocus( bool b = true );
  //
    /** sets the number of bytes per line, switching the resize style to KHE::NoResize */
    virtual void setNoOfBytesPerLine( int NoCpL );
    /** sets absolut offset of the data */
    void setStartOffset( int SO );
    /** sets offset of the char in the upper left corner */
    void setFirstLineOffset( int FLO );
  // value column parameters
    /** sets the spacing between the bytes in the value column
      * @param BSW spacing between the bytes in pixels
      * default is 3
      */
    void setByteSpacingWidth( int/*KPixelX*/ BSW ) ;
    /** sets the number of grouped bytes in the value column
      * @param NoGB numbers of grouped bytes, 0 means no grouping
      * default is 4
      */
    void setNoOfGroupedBytes( int NoGB );
    /** sets the spacing between the groups of bytes in the value column
      * @param GSW spacing between the groups in pixels
      * default is 9
      */
    void setGroupSpacingWidth( int/*KPixelX*/ GSW );
    /** sets the spacing in the middle of a binary byte in the value column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    void setBinaryGapWidth( int BGW );
    /** sets the spacing in the value column
      * @param ByteSpacingWidth spacing between the bytes in pixels
      * @param NoOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * @param GroupSpacingWidth spacing between the groups in pixels
      * Default is 4 for NoOfGroupedBytes
      */
    void setBufferSpacing( KPixelX ByteSpacingWidth, int NoOfGroupedBytes = 0, KPixelX GroupSpacingWidth = 0 );
    /** sets the format of the value column. Default is KHE::HexadecimalCoding */
    void setCoding( KCoding C );
  // char column parameters
    /** sets whether control chars or "non-printing" chars should be displayed in the char column
      * with their corresponding character. Currently this simply means all chars with value <32,
      * as known from the ASCII.
      * @param SU
      * returns true if there was a change
      */
    void setShowsNonprinting( bool SU = true );
    /** sets the substitute character for "non-printing" chars
      * returns true if there was a change
      */
    void setSubstituteChar( QChar SC );
    /** sets the undefined character for "undefined" chars
     * returns true if there was a change
     */
    void setUndefinedChar( QChar UC );
    /** sets the encoding of the char column. Default is KHE::LocalEncoding.
      * If the encoding is not available the format will not be changed. */
    void setEncoding( KEncoding C );
    /** sets the encoding of the char column. Default is KHE::LocalEncoding.
      * If the encoding is not available the format will not be changed.
      * @param Encoding name of the encoding
      */
    void setEncoding( const QString& Encoding );

  // interaction
    /** de-/selects all data */
    void selectAll( bool select );
    /** selects word at index, returns true if there is one */
    bool selectWord( /*unsigned*/ int Index /*, Chartype*/ );
    /** removes the selected data, takes care of the cursor */
    virtual void removeSelectedData();
    /** inserts */
    virtual void insert( const QByteArray &D );

  // clipboard interaction
    virtual void copy();
    virtual void cut();
    virtual void paste();

  // zooming
    virtual void zoomIn( int PointInc );
    virtual void zoomIn();
    virtual void zoomOut( int PointInc );
    virtual void zoomOut();
    virtual void zoomTo( int PointSize );
    virtual void unZoom();
    void setZoomLevel( double Level );

  // cursor control
    /** we have focus again, start the timer */
    virtual void startCursor();
    /** we lost focus, stop the timer */
    virtual void stopCursor();
    /** simply pauses any blinking, i.e. ignores any calls to blinkCursor */
    virtual void pauseCursor();
    /** undoes pauseCursor */
    virtual void unpauseCursor();


  Q_SIGNALS:
    /** Index of the byte that was clicked */
    void clicked( int Index );
    /** Index of the byte that was double clicked */
    void doubleClicked( int Index );

    void cursorPositionChanged( int Index );
    void readOnlyChanged( bool isReadOnly );
    /** selection has changed */
    void selectionChanged( bool HasSelection );
    /** there is a cut available or not */
    void cutAvailable( bool Really );
    /** there is a copy available or not */
    void copyAvailable( bool Really );
    /** */
    void charCodecChanged( const QString &codecName );
    /** */
    void valueCodingChanged( int coding );


  protected: // QWidget API
    virtual void keyPressEvent( QKeyEvent *keyEvent );
    virtual void resizeEvent( QResizeEvent *resizeEvent );
    virtual void showEvent( QShowEvent *showEvent );
    virtual void focusInEvent( QFocusEvent *focusEvent );
    virtual void focusOutEvent( QFocusEvent *focusEvent );
    /// reimplemented to catch Tab and BackTab keys, which otherwise gets stolen
    virtual bool event( QEvent *event );

  protected: // QAbstractScrollArea API
    virtual void mousePressEvent( QMouseEvent *e );
    virtual void mouseReleaseEvent( QMouseEvent * e );
    virtual void mouseMoveEvent( QMouseEvent *e );
    virtual void mouseDoubleClickEvent( QMouseEvent * e );
    virtual void dragEnterEvent( QDragEnterEvent *e );
    virtual void dragMoveEvent( QDragMoveEvent *e );
    virtual void dragLeaveEvent( QDragLeaveEvent * );
    virtual void dropEvent( QDropEvent *e );
    virtual void wheelEvent( QWheelEvent *e );
//    virtual void contextMenuEvent( QContextMenuEvent *e );

  protected: // ColumnsView API
    virtual void setNoOfLines( int NewNoOfLines );


  protected: // element accessor functions
    ValueByteArrayColumnRenderer& valueColumn();
    CharByteArrayColumnRenderer& charColumn();
    AbstractByteArrayColumnRenderer& activeColumn();
    AbstractByteArrayColumnRenderer& inactiveColumn();
    const ValueByteArrayColumnRenderer& valueColumn()    const;
    const CharByteArrayColumnRenderer& charColumn()   const;
    const AbstractByteArrayColumnRenderer& activeColumn() const;
    const AbstractByteArrayColumnRenderer& inactiveColumn() const;

  protected: // atomic ui operations
    /** handles screen update in case of a change to any of the width sizes
      */
    void updateViewByWidth();
    /** repaints all the parts that are signed as changed */
    void updateChanged();

  protected: // drawing related operations
    /** recreates the cursor pixmaps and paints active and inactive cursors if doable */
    void updateCursors();
    void createCursorPixmaps();
    /** draws the blinking cursor or removes it */
    void drawActiveCursor( QPainter *painter );
    void drawInactiveCursor( QPainter *painter );
    void updateCursor( const AbstractByteArrayColumnRenderer &Column );

  protected: // partial operations
    void handleMouseMove( const QPoint& Point );
    /** sets ChangedRange to the range of VisibleRange that is actually changed
      * @return true if there was a change within the visible range
      */
    bool hasChanged( const CoordRange &VisibleRange, CoordRange *ChangedRange ) const;
    void handleInternalDrag( QDropEvent *e );

  protected:
    /** recalcs all dependant values with the actual NoOfBytesPerLine  */
    void adjustToLayoutNoOfBytesPerLine();
    /** recalcs a layout due to the resize style that fits into the view size
      * and updates the dependant values
      */
    void adjustLayoutToSize();
    /** calls updateContent for the Column */
    void updateColumn( ColumnRenderer &Column );
    void ensureVisible( const AbstractByteArrayColumnRenderer &Column, const Coord &Coord );
    void emitSelectionSignals();
    void adaptController();

  protected Q_SLOTS:
    /** gets called by the cursor blink timer */
    void blinkCursor();
    /** gets called by the scroll timer (for mouse selection) */
    void autoScrollTimerDone();
    /** */
    void clipboardChanged();
    /** */
    void startDrag();

    void onByteArrayReadOnlyChange( bool isByteArrayReadOnly );
    void onContentsChanged( const KHE::ArrayChangeMetricsList &changeList );
    void updateRange( int Start, int End );
    void updateRange( const KHE::KSectionList &list );
    void onBookmarksChange( const QList<KHECore::KBookmark> &bookmarks );
    void onRevertedToVersionIndex( int versionIndex );

  protected Q_SLOTS: // QWidget API
    virtual void fontChange( const QFont &OldFont );


  protected:
    /** Buffer with the data */
    KHECore::KAbstractByteArrayModel *mByteArrayModel;

    /** holds the logical layout */
    ByteArrayTableLayout *mDataLayout;
    /** */
    KDataCursor *mDataCursor;
    /** */
    KDataRanges *mDataRanges;


  protected:
    OffsetColumnRenderer *mOffsetColumn;
    BorderColumnRenderer *mFirstBorderColumn;
    ValueByteArrayColumnRenderer  *mValueColumn;
    BorderColumnRenderer *mSecondBorderColumn;
    CharByteArrayColumnRenderer   *mCharColumn;

    /** points to the column with keyboard focus */
    AbstractByteArrayColumnRenderer *mActiveColumn;
    /** points to the column without keyboard focus (if there is) */
    AbstractByteArrayColumnRenderer *mInactiveColumn;

    /** the actual input controller */
    KController *mController;
    /** */
    KTabController *mTabController;
    /** */
    KNavigator *mNavigator;
    /** */
    KValueEditor *mValueEditor;
    /** */
    KCharEditor *mCharEditor;

  protected:
    /** Timer that controls the blinking of the cursor */
    QTimer *mCursorBlinkTimer;
    /** Timer that triggers ensureCursorVisible function calls */
    QTimer *mScrollTimer;
/*     QTimer *ChangeIntervalTimer, */
    /** Timer to start a drag */
    QTimer *mDragStartTimer;
    /** timer to measure whether the time between a double click and the following counts for a tripleclick */
    QTimer *mTrippleClickTimer;

    /** object to store the blinking cursor pixmaps */
    KCursor *mCursorPixmaps;
    /** */
    KHECore::KCharCodec *mCharCodec;

  protected:
    /** point at which the current double click happended (used by TrippleClick) */
    QPoint mDoubleClickPoint;
    /** line in which the current double click happended (used by TrippleClick) */
    int mDoubleClickLine;
    /** point at which the current dragging started */
    QPoint mDragStartPoint;
    /** */
    QClipboard::Mode mClipboardMode;
    /** font size as set by user (used for zooming) */
    int mDefaultFontSize;
    /** stores the cursor before it follows a drag*/
    int mBeforeDragCursorPos;
    bool mBeforeDragCursorIsBehind:1;
    bool mCursorIsMovedByDrag:1;


  protected: // parameters
    /** style of resizing */
    KResizeStyle mResizeStyle;
    /** */
    KEncoding mCharEncoding;

    /** flag whether the widget is set to readonly. Cannot override the databuffer's setting, of course. */
    bool mReadOnly:1;
    /** flag if only overwrite is allowed */
    bool mOverWriteOnly:1;
    /** flag if overwrite mode is active */
    bool mOverWrite:1;
    /** flag if a mouse button is pressed */
    bool mMousePressed:1;
    /** flag if a double click is happening */
    bool mInDoubleClick:1;
    /** flag if a Drag'n'Drop is happening */
    bool mInDnD:1;
    /** flag if a drag might have started */
    bool mDragStartPossible:1;
    /** flag if the cursor should be invisible */
    bool mCursorPaused:1;
    /** flag if the cursor is visible */
    bool mBlinkCursorVisible:1;
    /** flag whether the font is changed due to a zooming */
    bool mInZooming:1;

  private:
    class Private;
    Private * const d;

  private: // Disabling copy constructor and operator= - not useful
    KByteArrayView( const KByteArrayView & );
    KByteArrayView &operator=( const KByteArrayView & );
};

}

#endif
