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

#ifndef KHE_UI_BYTEARRAYCOLUMNVIEW_H
#define KHE_UI_BYTEARRAYCOLUMNVIEW_H

// lib
#include "abstractbytearrayview.h"


namespace KHEUI
{
class KValueEditor;


class ByteArrayColumnViewPrivate;

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

class OKTETAGUI_EXPORT ByteArrayColumnView : public AbstractByteArrayView
{
  friend class KTabController;
  friend class KNavigator;
  friend class KEditor;
  friend class KValueEditor;
  friend class KCharEditor;
  friend class Dropper;

  Q_OBJECT
  Q_PROPERTY( bool OverwriteMode READ isOverwriteMode WRITE setOverwriteMode )
  Q_PROPERTY( bool OverwriteOnly READ isOverwriteOnly WRITE setOverwriteOnly )
  Q_PROPERTY( bool Modified READ isModified WRITE setModified DESIGNABLE false )
  Q_PROPERTY( bool ReadOnly READ isReadOnly WRITE setReadOnly )

  Q_PROPERTY( bool TabChangesFocus READ tabChangesFocus WRITE setTabChangesFocus )

  //Q_PROPERTY( bool hasSelectedData READ hasSelectedData )
  //Q_PROPERTY( QByteArray SelectedData READ selectedData )
  Q_PROPERTY( ResizeStyle ResizeStyle READ resizeStyle WRITE setResizeStyle )
  Q_PROPERTY( int NoOfBytesPerLine READ noOfBytesPerLine WRITE setNoOfBytesPerLine )
  Q_PROPERTY( int StartOffset READ startOffset WRITE setStartOffset )
  Q_PROPERTY( int FirstLineOffset READ firstLineOffset WRITE setFirstLineOffset )
  Q_PROPERTY( int VisibleBufferColumns READ visibleByteArrayCodings WRITE setVisibleByteArrayCodings )
  Q_PROPERTY( bool ByteTypeColored READ isByteTypeColored WRITE setByteTypeColored )
  // value column
  Q_PROPERTY( ValueCoding Coding READ valueCoding WRITE setValueCoding )
  Q_PROPERTY( int ByteSpacingWidth READ byteSpacingWidth WRITE setByteSpacingWidth )
  Q_PROPERTY( int NoOfGroupedBytes READ noOfGroupedBytes WRITE setNoOfGroupedBytes )
  Q_PROPERTY( int GroupSpacingWidth READ groupSpacingWidth WRITE setGroupSpacingWidth )
  Q_PROPERTY( int BinaryGapWidth READ binaryGapWidth WRITE setBinaryGapWidth )
  // char column
  Q_PROPERTY( bool ShowNonprinting READ showsNonprinting WRITE setShowsNonprinting )
  Q_PROPERTY( QChar SubstituteChar READ substituteChar WRITE setSubstituteChar )

  public:
    explicit ByteArrayColumnView( KHECore::KAbstractByteArrayModel* byteArrayModel = 0, QWidget* parent = 0 );
    virtual ~ByteArrayColumnView();

  public: // AbstractByteArrayView
    /** */
    virtual void setByteArrayModel( KHECore::KAbstractByteArrayModel* byteArrayModel );

  public: // ColumnsView API
    virtual void renderColumns( QPainter* painter, int cx, int cy, int cw, int ch );

  public: // QWidget API
    virtual QSize minimumSizeHint() const;

  public: // value access
    bool tabChangesFocus() const;

    CodingTypeId activeCoding() const;
    bool offsetColumnVisible() const;
    int visibleByteArrayCodings() const;

  // value column
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

    bool isByteTypeColored() const;


  public: // logic value service
    /** calculates the number of bytes per line that fit into a widget with the given size
      * tests whether a vertical scroll bar is needed at all or not due to the given width
      * takes the frame width into account
      * @param TestSize Size the widget might have
      * @return number of bytes per line that fit into a widget with the given size
      */
//     int fittingBytesPerLine() const;
    /** detects the index of the byte at the given point
      * @param Point in viewport coordinate system
      * @return index of the byte that covers the point
      */
    int indexByPoint(const QPoint& point ) const;

  public: // modification access
    /** puts the cursor in the column at the pos of Point (in absolute coord), does not handle the drawing */
    void placeCursor( const QPoint& point );
    /***/
    void setActiveCoding( CodingTypeId codingId );
//    void repaintByte( int row, int column, bool Erase = true );
//    void updateByte( int row, int column );
//    void ensureByteVisible( int row, int column );

  public Q_SLOTS:
    /** switches the Offset column on/off */
    void toggleOffsetColumn( bool offsetColumnVisible );
    /** */
    void setVisibleByteArrayCodings( int visibleByteArrayCodings );
    /** scrolls the view as much as needed to have the cursor fully visible */
    void ensureCursorVisible();

  // setting parameters
    /** sets whether on a tab key there should be switched from the char column back to the value column
      * or be switched to the next focusable widget. Default is false
      */
    void setTabChangesFocus( bool tabChangesFocus = true );
  //
  // value column parameters
    /** sets the spacing between the bytes in the value column
      * @param BSW spacing between the bytes in pixels
      * default is 3
      */
    void setByteSpacingWidth( int/*KPixelX*/ byteSpacingWidth ) ;
    /** sets the number of grouped bytes in the value column
      * @param NoGB numbers of grouped bytes, 0 means no grouping
      * default is 4
      */
    void setNoOfGroupedBytes( int noOfGroupedBytes );
    /** sets the spacing between the groups of bytes in the value column
      * @param GSW spacing between the groups in pixels
      * default is 9
      */
    void setGroupSpacingWidth( int/*KPixelX*/ groupSpacingWidth );
    /** sets the spacing in the middle of a binary byte in the value column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    void setBinaryGapWidth( int binaryGapWidth );
    /** sets the spacing in the value column
      * @param ByteSpacingWidth spacing between the bytes in pixels
      * @param NoOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * @param GroupSpacingWidth spacing between the groups in pixels
      * Default is 4 for NoOfGroupedBytes
      */
    void setBufferSpacing( int/*KPixelX*/ byteSpacingWidth, int noOfGroupedBytes = 0, int/*KPixelX*/ groupSpacingWidth = 0 );
    /** sets the format of the value column. Default is KHE::HexadecimalCoding */
    void setValueCoding( ValueCoding valueCoding );
  // char column parameters
    /** sets whether control chars or "non-printing" chars should be displayed in the char column
      * with their corresponding character. Currently this simply means all chars with value <32,
      * as known from the ASCII.
      * @param SU
      * returns true if there was a change
      */
    void setShowsNonprinting( bool showsNonprinting = true );
    /** sets the substitute character for "non-printing" chars
      * returns true if there was a change
      */
    void setSubstituteChar( QChar substituteChar );
    /** sets the undefined character for "undefined" chars
     * returns true if there was a change
     */
    void setUndefinedChar( QChar undefinedChar );
    /** sets the encoding of the char column. Default is KHE::LocalEncoding.
      * If the encoding is not available the format will not be changed. */
    void setCharCoding( CharCoding charCoding );
    /** sets the encoding of the char column. Default is KHE::LocalEncoding.
      * If the encoding is not available the format will not be changed.
      * @param Encoding name of the encoding
      */
    void setCharCoding( const QString& charCodingName );
    void setByteTypeColored( bool isColored );

  // cursor control
    /** we have focus again, start the timer */
    void startCursor();
    /** we lost focus, stop the timer */
    void stopCursor();
    /** simply pauses any blinking, i.e. ignores any calls to blinkCursor */
    void pauseCursor();
    /** undoes pauseCursor */
    void unpauseCursor();

  protected: // QAbstractScrollArea API
    virtual void mousePressEvent( QMouseEvent* mousePressEvent );
    virtual void mouseReleaseEvent( QMouseEvent* mouseReleaseEvent );
    virtual void mouseMoveEvent( QMouseEvent* mouseMoveEvent );
    virtual void mouseDoubleClickEvent( QMouseEvent* mouseDoubleClickEvent );
    virtual void dragEnterEvent( QDragEnterEvent* dragEnterEvent );
    virtual void dragMoveEvent( QDragMoveEvent* dragMoveEvent) ;
    virtual void dragLeaveEvent( QDragLeaveEvent* dragLeaveEvent );
    virtual void dropEvent( QDropEvent* dropEvent );
//    virtual void contextMenuEvent( QContextMenuEvent* contextMenuEvent );

  protected: // element accessor functions
    KValueEditor* valueEditor() const;

  protected:
    void emitSelectionSignals();
    void updateChanged();
    void updateCursors();

  protected: // Q_SLOTS QWidget API
    virtual void fontChange( const QFont& oldFont );

  private:
    Q_DECLARE_PRIVATE( ByteArrayColumnView )
    Q_PRIVATE_SLOT( d_func(), void blinkCursor() )
    Q_PRIVATE_SLOT( d_func(), void autoScrollTimerDone() )
//     Q_PRIVATE_SLOT( d_func(), void clipboardChanged() )
    Q_PRIVATE_SLOT( d_func(), void startDrag() )

  private: // Disabling copy constructor and operator= - not useful
    ByteArrayColumnView( const ByteArrayColumnView & );
    ByteArrayColumnView &operator=( const ByteArrayColumnView & );
};

}

#endif
