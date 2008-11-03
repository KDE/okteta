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

#ifndef KHE_UI_ABSTRACTBYTEARRAYVIEW_H
#define KHE_UI_ABSTRACTBYTEARRAYVIEW_H

// lib
#include "columnsview.h"

namespace KHECore {
class KBookmark;
class ValueCodec;
class KCharCodec;
class KAbstractByteArrayModel;
}
namespace KHE {
class ArrayChangeMetricsList;
class KSection;
}

class QMimeData;
class QByteArray;

namespace KHEUI
{
class KController;
class AbstractWheelController;
class ByteArrayTableLayout;
class ByteArrayTableCursor;
class ByteArrayTableRanges;


class AbstractByteArrayViewPrivate;

// TODO: for now inherit from ColumnsView, but later on invert this,
// so it's AbstractByteArrayView < ColumnsView < {ByteArrayRowView,ByteArrayColumnView}
class OKTETAGUI_EXPORT AbstractByteArrayView : public ColumnsView
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

  //Q_PROPERTY( bool hasSelectedData READ hasSelectedData )
  //Q_PROPERTY( QByteArray SelectedData READ selectedData )
  Q_PROPERTY( bool TabChangesFocus READ tabChangesFocus WRITE setTabChangesFocus )
  Q_PROPERTY( ResizeStyle ResizeStyle READ resizeStyle WRITE setResizeStyle )
  Q_PROPERTY( int NoOfBytesPerLine READ noOfBytesPerLine WRITE setNoOfBytesPerLine )
  Q_PROPERTY( int StartOffset READ startOffset WRITE setStartOffset )
  Q_PROPERTY( int FirstLineOffset READ firstLineOffset WRITE setFirstLineOffset )
  Q_PROPERTY( int VisibleCodings READ visibleCodings WRITE setVisibleCodings )
  Q_ENUMS( ValueCoding CharCoding ResizeStyle )

  public:
    enum ValueCoding { HexadecimalCoding=0, DecimalCoding=1, OctalCoding=2, BinaryCoding=3, MaxCodingId=0xFFFF };
    enum CharCoding { LocalEncoding=0, ISO8859_1Encoding=1, EBCDIC1047Encoding=2,
                     StartOfOwnEncoding=0x8000, MaxEncodingId=0xFFFF };
    enum ResizeStyle { NoResize=0, LockGrouping=1, FullSizeUsage=2, MaxResizeStyleId=0xFF };

    enum CodingTypeId { NoCodingId=0, ValueCodingId=1, CharCodingId=2, BothCodingsId=ValueCodingId|CharCodingId };

  public:
    virtual ~AbstractByteArrayView();

  public: // value access
    KHECore::KAbstractByteArrayModel* byteArrayModel() const;
    bool isModified() const;

    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;
    bool isReadOnly() const;

    /** returns the index of the cursor position */
    int cursorPosition() const;
    /***/
    bool isCursorBehind() const;

    int startOffset() const;
    int firstLineOffset() const;
    int noOfBytesPerLine() const;

    ResizeStyle resizeStyle() const;

    bool tabChangesFocus() const;

    int visibleCodings() const;
    CodingTypeId activeCoding() const;

    /** returns true if there is a selected range in the array */
    bool hasSelectedData() const;
    KHE::KSection selection() const;

    ValueCoding valueCoding() const;
    /**
      * @return encoding used for the chars
      */
    CharCoding charCoding() const;
    /**
     * @return name of the encoding used for the chars
     */
    const QString& charCodingName() const;

    double zoomLevel() const;

  public: // value access API
    virtual bool offsetColumnVisible() const = 0;

  // value column
    virtual int/*KPixelX*/ byteSpacingWidth() const = 0;
    virtual int noOfGroupedBytes() const = 0;
    virtual int/*KPixelX*/ groupSpacingWidth() const = 0;
    virtual int/*KPixelX*/ binaryGapWidth() const = 0;

  // char column
    /** reports if "non-printing" chars are displayed in the char column
      * with their original character. Default is false
      * @return @c true if original chars are displayed, otherwise @c false 
      */
    virtual bool showsNonprinting() const = 0;
    /** gives the used substitute character for "unprintable" chars, default is '.'
      * @return substitute character 
      */
    virtual QChar substituteChar() const = 0;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    virtual QChar undefinedChar() const = 0;

    virtual bool isByteTypeColored() const = 0;


  public:
    virtual void setByteArrayModel( KHECore::KAbstractByteArrayModel* byteArrayModel );

    /** switches the Offset column on/off */
    virtual void toggleOffsetColumn( bool offsetColumnVisible ) = 0;

  // setting parameters
  // value column parameters
    /** sets the spacing between the bytes in the value column
      * @param BSW spacing between the bytes in pixels
      * default is 3
      */
    virtual void setByteSpacingWidth( int/*KPixelX*/ byteSpacingWidth ) = 0;
    /** sets the number of grouped bytes in the value column
      * @param NoGB numbers of grouped bytes, 0 means no grouping
      * default is 4
      */
    virtual void setNoOfGroupedBytes( int noOfGroupedBytes ) = 0;
    /** sets the spacing between the groups of bytes in the value column
      * @param GSW spacing between the groups in pixels
      * default is 9
      */
    virtual void setGroupSpacingWidth( int/*KPixelX*/ groupSpacingWidth ) = 0;
    /** sets the spacing in the middle of a binary byte in the value column
      * @param BinaryGapW spacing in the middle of a binary in pixels
      * returns true if there was a change
      */
    virtual void setBinaryGapWidth( int binaryGapWidth ) = 0;
    /** sets the spacing in the value column
      * @param ByteSpacingWidth spacing between the bytes in pixels
      * @param NoOfGroupedBytes numbers of grouped bytes, 0 means no grouping
      * @param GroupSpacingWidth spacing between the groups in pixels
      * Default is 4 for NoOfGroupedBytes
      */
    virtual void setBufferSpacing( int/*KPixelX*/ byteSpacingWidth, int noOfGroupedBytes = 0, int/*KPixelX*/ groupSpacingWidth = 0 ) = 0;
    /** sets the format of the value column. Default is KHE::HexadecimalCoding */
    virtual void setValueCoding( ValueCoding valueCoding ) = 0;
  // char column parameters
    /** sets whether control chars or "non-printing" chars should be displayed in the char column
      * with their corresponding character. Currently this simply means all chars with value <32,
      * as known from the ASCII.
      * @param SU
      * returns true if there was a change
      */
    virtual void setShowsNonprinting( bool showsNonprinting = true ) = 0;
    /** sets the substitute character for "non-printing" chars
      * returns true if there was a change
      */
    virtual void setSubstituteChar( QChar substituteChar ) = 0;
    /** sets the undefined character for "undefined" chars
     * returns true if there was a change
     */
    virtual void setUndefinedChar( QChar undefinedChar ) = 0;
    /** sets the encoding of the char column. Default is KHE::LocalEncoding.
      * If the encoding is not available the format will not be changed. */
    virtual void setCharCoding( CharCoding charCoding ) = 0;
    /** sets the encoding of the char column. Default is KHE::LocalEncoding.
      * If the encoding is not available the format will not be changed.
      * @param Encoding name of the encoding
      */
    virtual void setCharCoding( const QString& charCodingName ) = 0;
    virtual void setByteTypeColored( bool isColored ) = 0;

  public:
    /** sets whether the data should be treated modified or not */
    void setModified( bool modified );

    /** sets the resizestyle for the value column. Default is KHE::FullSizeUsage */
    void setResizeStyle( ResizeStyle resizeStyle );
    /** sets whether the widget is readonly or not, Default is true.
      * If the databuffer which is worked on can't be written the widget stays readonly
      */
    void setReadOnly( bool readOnly );
    /** sets whether the widget is overwriteonly or not. Default is false. */
    void setOverwriteOnly( bool overwriteOnly );
    /** sets whether the widget is in overwrite mode or not. Default is true. */
    void setOverwriteMode( bool overwriteMode );

    /** sets the number of bytes per line, switching the resize style to KHE::NoResize */
    void setNoOfBytesPerLine( int noOfBytesPerLine );
    /** sets absolut offset of the data */
    void setStartOffset( int startOffset );
    /** sets offset of the char in the upper left corner */
    void setFirstLineOffset( int firstLineOffset );

    /** sets whether on a tab key there should be switched from the char column back to the value column
      * or be switched to the next focusable widget. Default is false
      */
    void setTabChangesFocus( bool tabChangesFocus = true );

    /***/
    void setActiveCoding( CodingTypeId codingId );
    /** */
    void setVisibleCodings( int visibleCodings );

  public:
    /** 
      * @return deep copy of the selected data 
      */
    QByteArray selectedData() const;
    QMimeData* selectionAsMimeData() const;

  public: // modification access
    void pasteData( const QMimeData* data );
    /** removes the selected data, takes care of the cursor */
    void removeSelectedData();
    /** inserts */
    void insert( const QByteArray& data );

    /** puts the cursor to the position of index, handles all drawing 
      * @param index 
      * @param isBehind 
      */
    void setCursorPosition( int index, bool isBehind = false );

    /** de-/selects all data */
    void selectAll( bool select );

    void setSelection( int start, int end );
    /** selects word at index, returns true if there is one */
    bool selectWord( /*unsigned*/ int index /*, Chartype*/ );
    /** scrolls the view as much as needed to have the cursor fully visible */
    void ensureCursorVisible();
    /** puts the cursor in the column at the pos of Point (in absolute coord), does not handle the drawing */
    void placeCursor( const QPoint& point );

  public:
    bool canReadData( const QMimeData* data ) const;

  public: // zooming
    void zoomIn( int pointInc );
    void zoomIn();
    void zoomOut( int pointDec );
    void zoomOut();
    void zoomTo( int pointSize );
    void unZoom();
    void setZoomLevel( double level );

  public: // cursor control
    /** we have focus again, start the timer */
    void startCursor();
    /** we lost focus, stop the timer */
    void stopCursor();
    /** simply pauses any blinking, i.e. ignores any calls to blinkCursor */
    void pauseCursor();
    /** undoes pauseCursor */
    void unpauseCursor();

  public:
  // clipboard interaction
    virtual void copy();
    virtual void cut();
    virtual void paste();

  public: // QWidget API
    virtual QSize sizeHint() const;


  Q_SIGNALS:
    /** Index of the byte that was clicked */
    void clicked( int index );
    /** Index of the byte that was double clicked */
    void doubleClicked( int index );

    void cursorPositionChanged( int index );
    /** */
    void overwriteModeChanged( bool newOverwriteMode );
    /** */
    void readOnlyChanged( bool isReadOnly );
    /** selection has changed */
    void selectionChanged( bool hasSelection );
    /** there is a cut available or not */
    void cutAvailable( bool Really );
    /** there is a copy available or not */
    void copyAvailable( bool Really );
    /** */
    void charCodecChanged( const QString& codecName );
    /** */
    void valueCodingChanged( int valueCoding );

  protected:
    void finishByteEdit();
    void emitSelectionSignals();
    void updateChanged();

  protected:
    const KHECore::ValueCodec* valueCodec() const;
    const KHECore::KCharCodec* charCodec() const;
    ByteArrayTableCursor* tableCursor() const;
    ByteArrayTableRanges* tableRanges() const;
    ByteArrayTableLayout* layout() const;

  protected: // QWidget API
    virtual void keyPressEvent( QKeyEvent* keyEvent );
    /// reimplemented to catch Tab and BackTab keys, which otherwise gets stolen
    virtual bool event( QEvent* event );
    virtual void showEvent( QShowEvent* showEvent );
    virtual void focusInEvent( QFocusEvent* focusEvent );
    virtual void focusOutEvent( QFocusEvent* focusEvent );
    virtual void resizeEvent( QResizeEvent* resizeEvent );
    virtual void dragEnterEvent( QDragEnterEvent* dragEnterEvent );
    virtual void dragMoveEvent( QDragMoveEvent* dragMoveEvent) ;
    virtual void dragLeaveEvent( QDragLeaveEvent* dragLeaveEvent );
    virtual void dropEvent( QDropEvent* dropEvent );

  protected: // QAbstractScrollArea API
    virtual void wheelEvent( QWheelEvent *e );

  protected: // ColumnsView API
    virtual void setNoOfLines( int newNoOfLines );

  protected: // Q_SLOTS QWidget API
    virtual void fontChange( const QFont& oldFont );

  protected:
    AbstractByteArrayViewPrivate* const d_ptr;
    AbstractByteArrayView( AbstractByteArrayViewPrivate* d, QWidget* parent );
  private:
    Q_DECLARE_PRIVATE( AbstractByteArrayView )
    Q_PRIVATE_SLOT( d_func(), void adaptController() )
    Q_PRIVATE_SLOT( d_func(), void onByteArrayReadOnlyChange(bool isByteArrayReadOnly) )
    Q_PRIVATE_SLOT( d_func(), void onContentsChanged( const KHE::ArrayChangeMetricsList &changeList ) )
    Q_PRIVATE_SLOT( d_func(), void onBookmarksChange( const QList<KHECore::KBookmark> &bookmarks ) )
    Q_PRIVATE_SLOT( d_func(), void onRevertedToVersionIndex( int versionIndex ) )
};

}

#endif
