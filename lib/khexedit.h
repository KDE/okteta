/***************************************************************************
                          khexedit.h  -  description
                             -------------------
    begin                : Die Mai 13 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHEXEDIT_H
#define KHEXEDIT_H

// qt specific
#include <qclipboard.h>
// app specific
// #include "khe.h"
#include "kcolumnsview.h"

class QTimer;

namespace KHE
{

class KCoordRange;

class KDataBuffer;

class KTextColumn;
class KHexColumn;
class KBufferColumn;
class KOffsetColumn;
class KBorderColumn;

class KBufferCursor;
class KBufferLayout;
class KBufferRanges;

class KBufferDrag;

class KCursor;

class KHexEditPrivate;


/** the main widget
  *
  *@author Friedrich W. H. Kossebau
  */

class KHexEdit : public KColumnsView
{
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
  //_PROPERTY( int undoDepth READ undoDepth WRITE setUndoDepth )
  //_PROPERTY( bool undoRedoEnabled READ isUndoRedoEnabled WRITE setUndoRedoEnabled )
  // hex column
  Q_PROPERTY( KCoding Coding READ coding WRITE setCoding )
  Q_PROPERTY( int ByteSpacingWidth READ byteSpacingWidth WRITE setByteSpacingWidth )
  Q_PROPERTY( int NoOfGroupedBytes READ noOfGroupedBytes WRITE setNoOfGroupedBytes )
  Q_PROPERTY( int GroupSpacingWidth READ groupSpacingWidth WRITE setGroupSpacingWidth )
  Q_PROPERTY( int BinaryGapWidth READ binaryGapWidth WRITE setBinaryGapWidth )
  // text column
  Q_PROPERTY( bool ShowUnprintable READ showUnprintable WRITE setShowUnprintable )
  Q_PROPERTY( QChar SubstituteChar READ substituteChar WRITE setSubstituteChar )

  public:
    enum KMoveAction { MoveBackward, MoveWordBackward, MoveForward, MoveWordForward,
                       MoveUp, MovePgUp, MoveDown, MovePgDown,
                       MoveLineStart, MoveHome, MoveLineEnd, MoveEnd };
    enum KKeyboardAction { ActionDelete, ActionWordDelete, ActionBackspace, ActionWordBackspace };

    enum KResizeStyle { NoResize, LockGrouping, FullSizeUsage };
    enum KCoding { HexadecimalCoding=0, DecimalCoding, OctalCoding, BinaryCoding, NoCoding };


  public:
    KHexEdit( KDataBuffer *Buffer = 0, QWidget *Parent = 0, const char *Name = 0, WFlags F = 0 );
    virtual ~KHexEdit();


  public: // KColumnsView API
    virtual void drawContents( QPainter *p, int cx, int cy, int cw, int ch );

  public: // QWidget API
//    void focusInEvent( QFocusEvent *FocusEvent ); // TODO: why don't these work?
//    void focusOutEvent( QFocusEvent *FocusEvent );
    virtual bool eventFilter( QObject *O, QEvent *E );

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;


  public: // value access
    /** returns true if there is a selected range in the array */
    bool hasSelectedData() const;

    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;
    bool isReadOnly() const;
    bool isModified() const;

    bool tabChangesFocus() const;

    KResizeStyle resizeStyle() const;
    int noOfBytesPerLine() const;
    int startOffset() const;
    int firstLineOffset() const;
  // hex column
    KCoding coding() const;
    int/*KPixelX*/ byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    int/*KPixelX*/ groupSpacingWidth() const;
    int/*KPixelX*/ binaryGapWidth() const;
  // text column
    /** returns true if "unprintable" chars (>32) are displayed in the text column
      * with their corresponding character, default is false
      */
    bool showUnprintable() const;
    /** returns the actually used substitute character for "unprintable" chars, default is '.' */
    QChar substituteChar() const;


  public: // logic value service
    /** returns the number of bytes per line that fit into a widget with the given size
      * tests whether a vertical scroll bar is needed at all or not due to the given width
      * takes the frame width into account
      *@param TestSize Size the widget might have
      */
    int fittingBytesPerLine( const QSize &TestSize ) const;
    /** returns the index of the byte that is displayed around the point (in viewport coordinate system)
      */
    int indexByPoint(const QPoint &Point ) const;

  public:
    /** returns a deep copy of the selected data */
    QByteArray selectedData() const;

  public:
    const KOffsetColumn& offsetColumn() const;
    const KHexColumn& hexColumn()    const;
    const KTextColumn& textColumn()   const;
    const KBufferColumn& activeColumn() const;
    const KBufferColumn& inactiveColumn() const;


  public: // modification access
    /** moves the cursor according to the action, handles all drawing */
    void moveCursor( KMoveAction Action );
    /** puts the cursor to the position of index, handles all drawing */
    void setCursorPosition( int Index );
    /** puts the cursor in the column at the pos of Point (in absolute coord), does not handle the drawing */
    void placeCursor( const QPoint &Point );
//    void repaintByte( int row, int column, bool Erase = true );
//    void updateByte( int row, int column );
//    void ensureByteVisible( int row, int column );

  public slots:
    /** */
    void setDataBuffer( KDataBuffer *B );

//    /** switches the Offset column on/off */
//    void toggleOffsetColumn( bool Visible );
    /** scrolls the view as much as needed to have the cursor fully visible */
    void ensureCursorVisible();

  // setting parameters
    /** sets the resizestyle for the hex column. Default is KHE::FullSizeUsage */
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
    /** sets whether on a tab key there should be switched from the text column back to the hex column
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
  // hex column parameters
    /** sets the spacing between the bytes in the hex column
      * @param BSW spacing between the bytes in pixels
      * default is 3
      */
    void setByteSpacingWidth( int/*KPixelX*/ BSW ) ;
    /** sets the number of grouped bytes in the hex column
      * @param NoGB numbers of grouped bytes, 0 means no grouping
      * default is 4
      */
    void setNoOfGroupedBytes( int NoGB );
    /** sets the spacing between the groups of bytes in the hex column
      * @param GSW spacing between the groups in pixels
      * default is 9
      */
    void setGroupSpacingWidth( int/*KPixelX*/ GSW );
    /** sets the spacing in the middle of a binary byte in the hex column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    void setBinaryGapWidth( int BGW );
    /** sets the spacing in the hex column
      * @param ByteSpacingWidth spacing between the bytes in pixels
      * @param NoOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * @param GroupSpacingWidth spacing between the groups in pixels
      * Default is 4 for NoOfGroupedBytes
      */
    void setBufferSpacing( KPixelX ByteSpacingWidth, int NoOfGroupedBytes = 0, KPixelX GroupSpacingWidth = 0 );
    /** sets the format of the hex column. Default is KHE::HexadecimalCoding */
    void setCoding( KCoding C );
  // text column parameters
    /** sets whether "unprintable" chars (>32) should be displayed in the text column
      * with their corresponding character.
      * @param SU
      * returns true if there was a change
      */
    void setShowUnprintable( bool SU = true );
    /** sets the substitute character for "unprintable" chars
      * returns true if there was a change
      */
    void setSubstituteChar( QChar SC );

  // interaction
    /** de-/selects all data */
    void selectAll( bool select );
    /**  */
    void moveCursor( KMoveAction Action, bool Select );
    /** executes keyboard Action \a Action. This is normally called by a key event handler. */
    void doKeyboardAction( KKeyboardAction Action );
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

  // cursor control
    /** we have focus again, start the timer */
    virtual void startCursor();
    /** we lost focus, stop the timer */
    virtual void stopCursor();
    /** simply pauses any blinking, i.e. ignores any calls to blinkCursor */
    virtual void pauseCursor( bool LeaveEdit = false );
    /** undoes pauseCursor */
    virtual void unpauseCursor();

  // byte editing
    /** steps inside editing the byte in the hec column */
    bool goInsideByte();
    /** */
    void goOutsideByte( bool MoveToNext = false );
    /** increases the byte in the buffer */
    bool incByte();
    /** increases the byte in the buffer */
    bool decByte();



  signals:
    /** Index of the byte that was clicked */
    void clicked( int Index );
    /** Index of the byte that was double clicked */
    void doubleClicked( int Index );

    void cursorPositionChanged( int Index );
    /** selection has changed */
    void selectionChanged();
    /** there is a cut available or not */
    void cutAvailable( bool Really );
    /** there is a copy available or not */
    void copyAvailable( bool Really );
    /** there has been a change to the buffer */
    void bufferChanged();
    /** the input was not even partially inserted */
    void inputFailed();


  protected: // QWidget API
    virtual void keyPressEvent( QKeyEvent *KeyEvent );
    virtual void resizeEvent( QResizeEvent *ResizeEvent );
    virtual void showEvent( QShowEvent *e );

  protected: // QScrollView API
    virtual void contentsMousePressEvent( QMouseEvent *e );
    virtual void contentsMouseReleaseEvent( QMouseEvent * e );
    virtual void contentsMouseMoveEvent( QMouseEvent *e );
    virtual void contentsMouseDoubleClickEvent( QMouseEvent * e );
    virtual void contentsDragEnterEvent( QDragEnterEvent *e );
    virtual void contentsDragMoveEvent( QDragMoveEvent *e );
    virtual void contentsDragLeaveEvent( QDragLeaveEvent * );
    virtual void contentsDropEvent( QDropEvent *e );
    virtual void contentsWheelEvent( QWheelEvent *e );
//    virtual void contentsContextMenuEvent( QContextMenuEvent *e );

  protected:
    KOffsetColumn& offsetColumn();
    KHexColumn& hexColumn();
    KTextColumn& textColumn();
    KBufferColumn& activeColumn();
    KBufferColumn& inactiveColumn();

    /** recreates the cursor pixmaps and paints active and inactive cursors if doable */
    void updateCursor();
    void createCursorPixmaps();
    void pointPainterToCursor( QPainter &Painter, const KBufferColumn &Column ) const;
    /** repaints all the parts that are signed as changed */
    void repaintChanged();
    /** draws the blinking cursor or removes it */
    void paintActiveCursor( bool CursorOn );
    void paintInactiveCursor( bool CursorOn );
    void paintEditedByte( bool Edited );

    void handleMouseMove( const QPoint& Point );
    bool hasChanged( const KCoordRange &VisibleRange, KCoordRange *ChangedRange ) const;
    void paintLine( QPainter *P, KBufferColumn *C, int Line, KSection Positions ) const;
    KBufferDrag *dragObject( bool F = false, QWidget *Parent = 0 ) const;
    void pasteFromSource( QMimeSource *Source );
    void removeData( KSection Indizes );
    bool handleByteEditKey( QKeyEvent *KeyEvent );
    bool handleLetter( QKeyEvent *KeyEvent );
    void syncEditedByte();

  protected:
    void fitToNoOfBytesPerLine();
    void fitInLine();
    void updateViewByWidth();
    void updateLength();

  protected slots:
    /** gets called by the cursor blink timer */
    void blinkCursor();
    /** gets called by the scroll timer (for mouse selection) */
    void autoScrollTimerDone();
    /** */
    void clipboardChanged();
    /** */
    void startDrag();

  protected slots: // QWidget API
    virtual void fontChange( const QFont &OldFont );


  protected:
    /** Buffer with the data */
    KDataBuffer *DataBuffer;

    /** holds the logical layout */
    KBufferLayout *BufferLayout;
    /** */
    KBufferCursor *BufferCursor;
    /** */
    KBufferRanges *BufferRanges;
    // TODO: Timer for syncing hexediting with textColumn... no needed?

  protected:
    KOffsetColumn *OffsetColumn;
    KBorderColumn *FirstBorderColumn;
    KHexColumn    *HexColumn;
    KBorderColumn *SecondBorderColumn;
    KTextColumn   *TextColumn;

    /** points to the column with keyboard focus */
    KBufferColumn *ActiveColumn;
    /** points to the column without keyboard focus (if there is) */
    KBufferColumn *InactiveColumn;

  protected:
    /** Timer that controls the blinking of the cursor */
    QTimer *CursorBlinkTimer;
    /** Timer that triggers ensureCursorVisible function calls */
    QTimer *ScrollTimer;
/*     QTimer *ChangeIntervalTimer, */
    /** Timer to start a drag */
    QTimer *DragStartTimer;
    /** timer to measure whether the time between a double click and the following counts for a tripleclick */
    QTimer *TrippleClickTimer;

    /** object to store the blinking cursor pixmaps */
    KCursor *CursorPixmaps;
    /** buffer with the  */
    char *ByteBuffer;
    /** stores the number of actual digits */
    int LengthOfByteBuffer;
    /** */
    unsigned char EditValue;
    /** stores the old byte value */
    unsigned char OldValue;

  protected:
    /** */
    QPoint MousePoint;
    QPoint OldMousePoint;
    QPoint DoubleClickPoint;
    QPoint DragStartPoint;
    int DoubleClickLine;
    QClipboard::Mode ClipboardMode;
    int DefaultFontSize;

  protected: // parameters
    /** style of resizing */
    KResizeStyle ResizeStyle;
    /** flag if tab key should be ignored */
    bool TabChangesFocus:1;
    /** flag whether the widget is set to readonly. Cannot override the databuffer's setting, of course. */
    bool ReadOnly:1;
    /** */
    bool OverWriteOnly:1;
    /** */
    bool OverWrite:1;
    /** flag if a mouse button is pressed */
    bool MousePressed:1;
    /** flag if a double click is happening */
    bool InDoubleClick:1;
    /** flag if a Drag'n'Drop is happening */
    bool InDnD:1;
    /** flag if a drag might have started */
    bool DragStartPossible:1;
    /** flag if the cursor should be invisible */
    bool CursorPaused:1;
    /** flag if the cursor is visible */
    bool BlinkCursorVisible:1;
    /** flag whether the font is changed due to a zooming */
    bool InZooming:1;
    /** flag whether we are in editing mode */
    bool InEditMode:1;
    /** flag whether byte edit mode was reached by inserting */
    bool EditModeByInsert:1;

  private:
    /** the binary compatibility saving helper */
    KHexEditPrivate* d;

  private: // Disabling copy constructor and operator= - not useful
    KHexEdit( const KHexEdit & );
    KHexEdit &operator=( const KHexEdit & );
};


inline const KOffsetColumn& KHexEdit::offsetColumn()   const { return *OffsetColumn; }
inline const KHexColumn& KHexEdit::hexColumn()         const { return *HexColumn; }
inline const KTextColumn& KHexEdit::textColumn()       const { return *TextColumn; }
inline const KBufferColumn& KHexEdit::activeColumn()   const { return *ActiveColumn; }
inline const KBufferColumn& KHexEdit::inactiveColumn() const { return *InactiveColumn; }

inline KOffsetColumn& KHexEdit::offsetColumn()   { return *OffsetColumn; }
inline KHexColumn& KHexEdit::hexColumn()         { return *HexColumn; }
inline KTextColumn& KHexEdit::textColumn()       { return *TextColumn; }
inline KBufferColumn& KHexEdit::activeColumn()   { return *ActiveColumn; }
inline KBufferColumn& KHexEdit::inactiveColumn() { return *InactiveColumn; }

}

#endif
