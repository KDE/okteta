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
  Q_OBJECT
  Q_ENUMS( ValueCoding CharCoding ResizeStyle )

  public:
    enum ValueCoding { HexadecimalCoding=0, DecimalCoding=1, OctalCoding=2, BinaryCoding=3, MaxCodingId=0xFFFF };
    enum CharCoding { LocalEncoding=0, ISO8859_1Encoding=1, EBCDIC1047Encoding=2,
                     StartOfOwnEncoding=0x8000, MaxEncodingId=0xFFFF };
    enum ResizeStyle { NoResize=0, LockGrouping=1, FullSizeUsage=2, MaxResizeStyleId=0xFF };

    enum CodingTypeId { ValueCodingId=1, CharCodingId=2 };

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

  public:
    virtual void setByteArrayModel( KHECore::KAbstractByteArrayModel* byteArrayModel );
    /** sets whether the data should be treated modified or not */
    virtual void setModified( bool modified );

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
    /** sets whether the widget is readonly or not, Default is true.
      * If the databuffer which is worked on can't be written the widget stays readonly
      */
//     void setReadOnly( bool readOnly );
    /** sets whether the widget is overwriteonly or not. Default is false. */
//     void setOverwriteOnly( bool overwriteOnly );
    /** sets whether the widget is in overwrite mode or not. Default is true. */
//     void setOverwriteMode( bool overwriteMode );

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
