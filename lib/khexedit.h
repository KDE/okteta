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
#include "ksection.h" // TODO: try to move this out of this API

class QTimer;

namespace KHE
{

class KCoordRange;

class KDataBuffer;

class KBufferColumn;
class KOffsetColumn;
class KBorderColumn;

class KBufferCursor;
class KBufferLayout;
class KBufferRanges;

class KBufferDrag;

class KCursor;

class KHexEditPrivate;

//enum KResizeStyle { NoResize, LockGrouping, FullSizeUsage };

/** the main widget
  *
  *@author Friedrich W. H. Kossebau
  */

class KHexEdit : public KColumnsView
{
  Q_OBJECT
  Q_ENUMS( KResizeStyle KCoding )
  Q_PROPERTY( KResizeStyle ResizeStyle READ resizeStyle WRITE setResizeStyle )
  Q_PROPERTY( KCoding Coding READ coding WRITE setCoding )
  Q_PROPERTY( int NoOfBytesPerLine READ noOfBytesPerLine WRITE setNoOfBytesPerLine )
  Q_PROPERTY( bool TabChangesFocus READ tabChangesFocus WRITE setTabChangesFocus )
  Q_PROPERTY( bool OverwriteMode READ isOverwriteMode WRITE setOverwriteMode )
  Q_PROPERTY( bool OverwriteOnly READ isOverwriteOnly WRITE setOverwriteOnly )
  Q_PROPERTY( bool Modified READ isModified WRITE setModified DESIGNABLE false )
  Q_PROPERTY( bool ReadOnly READ isReadOnly WRITE setReadOnly )

  //Q_PROPERTY( bool hasSelectedData READ hasSelectedData )
  //Q_PROPERTY( QByteArray SelectedData READ selectedData )
  Q_PROPERTY( int StartOffset READ startOffset WRITE setStartOffset )
  Q_PROPERTY( int FirstLineOffset READ firstLineOffset WRITE setFirstLineOffset )
  //_PROPERTY( int undoDepth READ undoDepth WRITE setUndoDepth )
  //_PROPERTY( bool undoRedoEnabled READ isUndoRedoEnabled WRITE setUndoRedoEnabled )
  Q_PROPERTY( int ByteSpacingWidth READ byteSpacingWidth WRITE setByteSpacingWidth )
  Q_PROPERTY( int NoOfGroupedBytes READ noOfGroupedBytes WRITE setNoOfGroupedBytes )
  Q_PROPERTY( int GroupSpacingWidth READ groupSpacingWidth WRITE setGroupSpacingWidth )
  Q_PROPERTY( int BinaryGapWidth READ binaryGapWidth WRITE setBinaryGapWidth )

  public:
    enum KMoveAction { MoveBackward, MoveWordBackward, MoveForward, MoveWordForward,
                       MoveUp, MovePgUp, MoveDown, MovePgDown,
                       MoveLineStart, MoveHome, MoveLineEnd, MoveEnd };
    enum KKeyboardAction { ActionDelete, ActionWordDelete, ActionBackspace, ActionWordBackspace };

    enum KResizeStyle { NoResize, LockGrouping, FullSizeUsage };
    enum KCoding { ASCIICoding=0, HexadecimalCoding, DecimalCoding, OctalCoding, BinaryCoding, NoCoding };


  protected:
    static const int NoOfBufferColumns = 2;

  public:
    KHexEdit( KDataBuffer *Buffer = 0, QWidget *Parent = 0, const char *Name = 0, WFlags F = 0 );
    virtual ~KHexEdit();


  public: // QWidget API
//    void focusInEvent( QFocusEvent *FocusEvent ); // TODO: why don't these work?
//    void focusOutEvent( QFocusEvent *FocusEvent );
    bool eventFilter( QObject *O, QEvent *E );

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;


  public: // value access
    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;
    bool isReadOnly() const;
    bool isModified() const;

    bool tabChangesFocus() const;

    int noOfBytesPerLine() const;
    int startOffset() const;
    int firstLineOffset() const;
    KResizeStyle resizeStyle() const;
    KCoding coding() const;
    int/*KPixelX*/ byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    int/*KPixelX*/ groupSpacingWidth() const;
    int/*KPixelX*/ binaryGapWidth() const;
    /** returns true if there is a selected range in the array */
    bool hasSelectedData() const;

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

  public: // modification access
    void moveCursor( KMoveAction Action );
    /** puts the cursor in the column at the pos of Point (in absolute coord)*/
    void placeCursor( const QPoint &Point );

//    void repaintByte( int row, int column, bool Erase = true );
//    void updateByte( int row, int column );
//    void ensureByteVisible( int row, int column );

    void insert( const QByteArray &D );

  public slots:
    /** */
    void setDataBuffer( KDataBuffer *B );

//    /** switches the Offset column on/off */
//    void toggleOffsetColumn( bool Visible );
    /** scrolls the view as much as needed to have the cursor fully visible */
    void ensureCursorVisible();

  // setting parameters
    /** sets the spacing in the hex column
      * @param ByteSpacingWidth spacing between the bytes in pixels
      * @param NoOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * @param GroupSpacingWidth spacing between the groups in pixels
      * Default is 4 for NoOfGroupedBytes
      */
    void setBufferSpacing( KPixelX ByteSpacingWidth, int NoOfGroupedBytes = 0, KPixelX GroupSpacingWidth = 0 );
    /** sets the format of the hex column. Default is KHE::HexadecimalCoding */
    void setCoding( KCoding C );
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
    virtual void setTabChangesFocus( bool b );
    /** sets the number of bytes per line, switching the resize style to KHE::NoResize */
    virtual void setNoOfBytesPerLine( int NoCpL );
    /** sets absolut offset of the data */
    void setStartOffset( int SO );
    /** sets offset of the char in the upper left corner */
    void setFirstLineOffset( int FLO );
    /** */
    void setByteSpacingWidth( int BSW ) ;
    /** */
    void setNoOfGroupedBytes( int NoGB );
    /** */
    void setGroupSpacingWidth( int GSW );
    /** sets the spacing in the middle of a binary byte in the hex column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    void setBinaryGapWidth( int BGW );

  // interaction
    /**  */
    void moveCursor( KMoveAction Action, bool Select );
    /** executes keyboard Action \a Action. This is normally called by a key event handler. */
    void doKeyboardAction( KKeyboardAction Action );
    /** removes the selected data, takes care of the cursor */
    void removeSelectedData();
    /** de-/selects all data */
    void selectAll( bool select );

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

  // cursor blinking
    virtual void startCursorBlinking();
    virtual void stopCursorBlinking();
    virtual void pauseCursorBlinking();
    virtual void unpauseCursorBlinking();


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


  protected: // QWidget API
    virtual void keyPressEvent( QKeyEvent *KeyEvent );
    virtual void resizeEvent( QResizeEvent *ResizeEvent );

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
    KBufferColumn& hexColumn();
    KBufferColumn& textColumn();
    KBufferColumn& activeColumn();

    const KOffsetColumn& offsetColumn() const;
    const KBufferColumn& hexColumn()    const;
    const KBufferColumn& textColumn()   const;
    const KBufferColumn& activeColumn() const;

    void createCursorPixmaps();
    /** repaints all the parts that are signed as changed */
    void repaintChanged();
    /** draws the blinking cursor or removes it */
    void drawCursor( bool CursorOn );
    void handleMouseMove( const QPoint& Point );
    bool hasChanged( const KCoordRange &VisibleRange, KCoordRange *ChangedRange ) const;
    void paintLine( QPainter *P, KBufferColumn *C, int Line, KSection Positions ) const;
    KBufferDrag *dragObject( QWidget *Parent = 0 ) const;
    void pasteFromSource( QMimeSource *Source );
    void removeData( KSection Indizes );

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
    // TODO: Timer for syncing hexediting with textColumn

  protected:
    KOffsetColumn *OffsetColumn;
    KBufferColumn *BufferColumn[NoOfBufferColumns];
    KBorderColumn *BorderColumn[NoOfBufferColumns];

    /** points to the column with keyboard focus */
    KBufferColumn *ActiveColumn;

  protected:
    QTimer *CursorBlinkTimer;
    QTimer *ScrollTimer;
/*     QTimer *ChangeIntervalTimer, */
    QTimer *DragStartTimer;
    QTimer *TrippleClickTimer;

    KCursor *CursorPixmaps;

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
    /** */
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
    bool CursorHidden:1;
    /** flag if the cursor is visible */
    bool BlinkCursorVisible:1;
    /** flag whether teh font is changed due to a zooming */
    bool Zooming:1;

  private:
    /** the binary compatibility saving helper */
    KHexEditPrivate* d;

  private: // Disabling copy constructor and operator= - not useful
    KHexEdit( const KHexEdit & );
    KHexEdit &operator=( const KHexEdit & );
};

}

#endif
