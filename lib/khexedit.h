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


#ifndef KHE_UI_KHEXEDIT_H
#define KHE_UI_KHEXEDIT_H

// qt specific
#include <QtGui/QClipboard>
// lib specific
// #include "khe.h"
#include <ksection.h>
#include "kcolumnsview.h"

class QTimer;

namespace KHECore {
class KCharCodec;
class KAbstractByteArrayModel;
}

namespace KHEUI
{

class KCoordRange;

class KCharColumn;
class KValueColumn;
class KBufferColumn;
class KOffsetColumn;
class KBorderColumn;

class KBufferCoord;
class KBufferCursor;
class KBufferLayout;
class KBufferRanges;

class KController;
class KTabController;
class KNavigator;
class KValueEditor;
class KCharEditor;

class KCursor;

class KHexEditPrivate;


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

class KHEUI_EXPORT KHexEdit : public KColumnsView
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
  //_PROPERTY( int undoDepth READ undoDepth WRITE setUndoDepth )
  //_PROPERTY( bool undoRedoEnabled READ isUndoRedoEnabled WRITE setUndoRedoEnabled )
  // value column
  Q_PROPERTY( KCoding Coding READ coding WRITE setCoding )
  Q_PROPERTY( int ByteSpacingWidth READ byteSpacingWidth WRITE setByteSpacingWidth )
  Q_PROPERTY( int NoOfGroupedBytes READ noOfGroupedBytes WRITE setNoOfGroupedBytes )
  Q_PROPERTY( int GroupSpacingWidth READ groupSpacingWidth WRITE setGroupSpacingWidth )
  Q_PROPERTY( int BinaryGapWidth READ binaryGapWidth WRITE setBinaryGapWidth )
  // char column
  Q_PROPERTY( bool ShowUnprintable READ showUnprintable WRITE setShowUnprintable )
  Q_PROPERTY( QChar SubstituteChar READ substituteChar WRITE setSubstituteChar )

  public:
    enum KResizeStyle { NoResize=0, LockGrouping=1, FullSizeUsage=2, MaxResizeStyleId=0xFF };
    enum KCoding { HexadecimalCoding=0, DecimalCoding=1, OctalCoding=2, BinaryCoding=3, MaxCodingId=0xFFFF };
    enum KEncoding { LocalEncoding=0, ISO8859_1Encoding=1, EBCDIC1047Encoding=2,
                     StartOfOwnEncoding=0x8000, MaxEncodingId=0xFFFF };

    enum KBufferColumnId { ValueColumnId=1, CharColumnId=2 };


  public:
    KHexEdit( KHECore::KAbstractByteArrayModel *Model = 0, QWidget *Parent = 0 );
    virtual ~KHexEdit();


  public: // KColumnsView API
    virtual void drawColumns( QPainter *p, int cx, int cy, int cw, int ch );

  public: // QWidget API
//    void focusInEvent( QFocusEvent *FocusEvent ); // TODO: why don't these work?
//    void focusOutEvent( QFocusEvent *FocusEvent );
    virtual bool eventFilter( QObject *O, QEvent *E );

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
    KBufferColumnId cursorColumn() const;
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
    /** reports if "unprintable" chars (value<32) are displayed in the char column
      * with their original character. Default is false
      * @return @c true if original chars are displayed, otherwise @c false 
      */
    bool showUnprintable() const;
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
    /** 
      * @return deep copy of the selected data 
      */
    QByteArray selectedData() const;

  public: // modification access
    /** puts the cursor to the position of index, handles all drawing 
      * @param Index 
      */
    void setCursorPosition( int Index, bool Behind=false );
    /** puts the cursor in the column at the pos of Point (in absolute coord), does not handle the drawing */
    void placeCursor( const QPoint &Point );
    /***/
    void setCursorColumn( KBufferColumnId );
//    void repaintByte( int row, int column, bool Erase = true );
//    void updateByte( int row, int column );
//    void ensureByteVisible( int row, int column );

    void setSelection( int Start, int End );

    QMimeData *dragObject() const;
    void pasteFromSource( const QMimeData *Source );

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
    /** sets whether "unprintable" chars (>32) should be displayed in the char column
      * with their corresponding character.
      * @param SU
      * returns true if there was a change
      */
    void setShowUnprintable( bool SU = true );
    /** sets the substitute character for "unprintable" chars
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
    virtual void pauseCursor( bool LeaveEdit = false );
    /** undoes pauseCursor */
    virtual void unpauseCursor();


  Q_SIGNALS:
    /** Index of the byte that was clicked */
    void clicked( int Index );
    /** Index of the byte that was double clicked */
    void doubleClicked( int Index );

    void cursorPositionChanged( int Index );
    /** selection has changed */
    void selectionChanged( bool HasSelection );
    /** there is a cut available or not */
    void cutAvailable( bool Really );
    /** there is a copy available or not */
    void copyAvailable( bool Really );


  protected: // QWidget API
    virtual void keyPressEvent( QKeyEvent *KeyEvent );
    virtual void resizeEvent( QResizeEvent *ResizeEvent );
    virtual void showEvent( QShowEvent *e );

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

  protected: // KColumnsView API
    virtual void setNoOfLines( int NewNoOfLines );


  protected: // element accessor functions
    KValueColumn& valueColumn();
    KCharColumn& charColumn();
    KBufferColumn& activeColumn();
    KBufferColumn& inactiveColumn();
    const KValueColumn& valueColumn()    const;
    const KCharColumn& charColumn()   const;
    const KBufferColumn& activeColumn() const;
    const KBufferColumn& inactiveColumn() const;

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
    void pointPainterToCursor( QPainter &Painter, const KBufferColumn &Column ) const;
    /** draws the blinking cursor or removes it */
    void drawActiveCursor();
    void drawInactiveCursor();
    void updateCursor( const KBufferColumn &Column );

  protected: // partial operations
    void handleMouseMove( const QPoint& Point );
    /** sets ChangedRange to the range of VisibleRange that is actually changed
      * @return true if there was a change within the visible range
      */
    bool hasChanged( const KCoordRange &VisibleRange, KCoordRange *ChangedRange ) const;
    void handleInternalDrag( QDropEvent *e );

  protected:
    /** recalcs all dependant values with the actual NoOfBytesPerLine  */
    void adjustToLayoutNoOfBytesPerLine();
    /** recalcs a layout due to the resize style that fits into the view size
      * and updates the dependant values
      */
    void adjustLayoutToSize();
    /** calls updateContent for the Column */
    void updateColumn( KColumn &Column );
    void ensureVisible( const KBufferColumn &Column, const KBufferCoord &Coord );
    void emitSelectionSignals();

  protected Q_SLOTS:
    /** gets called by the cursor blink timer */
    void blinkCursor();
    /** gets called by the scroll timer (for mouse selection) */
    void autoScrollTimerDone();
    /** */
    void clipboardChanged();
    /** */
    void startDrag();

    void onContentsReplaced( int Pos, int RemovedLength, int InsertedLength );
    void onContentsMoved( int Destination, int Source, int MovedLength );
    void updateRange( int Start, int End );


  protected Q_SLOTS: // QWidget API
    virtual void fontChange( const QFont &OldFont );


  protected:
    /** Buffer with the data */
    KHECore::KAbstractByteArrayModel *ByteArrayModel;

    /** holds the logical layout */
    KBufferLayout *BufferLayout;
    /** */
    KBufferCursor *BufferCursor;
    /** */
    KBufferRanges *BufferRanges;


  protected:
    KOffsetColumn *OffsetColumn;
    KBorderColumn *FirstBorderColumn;
    KValueColumn  *ValueColumn;
    KBorderColumn *SecondBorderColumn;
    KCharColumn   *CharColumn;

    /** points to the column with keyboard focus */
    KBufferColumn *ActiveColumn;
    /** points to the column without keyboard focus (if there is) */
    KBufferColumn *InactiveColumn;

    /** the actual input controller */
    KController *Controller;
    /** */
    KTabController *TabController;
    /** */
    KNavigator *Navigator;
    /** */
    KValueEditor *ValueEditor;
    /** */
    KCharEditor *CharEditor;

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
    /** */
    KHECore::KCharCodec *Codec;

  protected:
    /** point at which the current double click happended (used by TrippleClick) */
    QPoint DoubleClickPoint;
    /** line in which the current double click happended (used by TrippleClick) */
    int DoubleClickLine;
    /** point at which the current dragging started */
    QPoint DragStartPoint;
    /** */
    QClipboard::Mode ClipboardMode;
    /** font size as set by user (used for zooming) */
    int DefaultFontSize;

  protected: // parameters
    /** style of resizing */
    KResizeStyle ResizeStyle;
    /** */
    KEncoding Encoding;

    /** flag whether the widget is set to readonly. Cannot override the databuffer's setting, of course. */
    bool ReadOnly:1;
    /** flag if only overwrite is allowed */
    bool OverWriteOnly:1;
    /** flag if overwrite mode is active */
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

  private:
    /** the binary compatibility saving helper */
    KHexEditPrivate* d;

  private: // Disabling copy constructor and operator= - not useful
    KHexEdit( const KHexEdit & );
    KHexEdit &operator=( const KHexEdit & );
};


inline const KValueColumn& KHexEdit::valueColumn()     const { return *ValueColumn; }
inline const KCharColumn& KHexEdit::charColumn()       const { return *CharColumn; }
inline const KBufferColumn& KHexEdit::activeColumn()   const { return *ActiveColumn; }
inline const KBufferColumn& KHexEdit::inactiveColumn() const { return *InactiveColumn; }

inline KValueColumn& KHexEdit::valueColumn()     { return *ValueColumn; }
inline KCharColumn& KHexEdit::charColumn()       { return *CharColumn; }
inline KBufferColumn& KHexEdit::activeColumn()   { return *ActiveColumn; }
inline KBufferColumn& KHexEdit::inactiveColumn() { return *InactiveColumn; }

}

#endif
