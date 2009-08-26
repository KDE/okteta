/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_ABSTRACTBYTEARRAYVIEW_P_H
#define OKTETA_ABSTRACTBYTEARRAYVIEW_P_H

// lib
#include "abstractbytearrayview.h"
#include "bytearraytablecursor.h"
#include "bytearraytableranges.h"
#include "bytearraytablelayout.h"
#include "controller/kvalueeditor.h"
#include "controller/ktabcontroller.h"
// Okteta core
#include <abstractbytearraymodel.h>
#include <charcodec.h>
// Qt
#include <QtGui/QClipboard>


namespace Okteta
{
class KNavigator;
class KCharEditor;

class Dropper;

class ZoomWheelController;


class AbstractByteArrayViewPrivate
{
  public:
    explicit AbstractByteArrayViewPrivate( AbstractByteArrayView* parent );
    virtual ~AbstractByteArrayViewPrivate();

  public: // value access
    Okteta::AbstractByteArrayModel* byteArrayModel() const;

    // TODO: needed?
    bool isModified() const;

    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;
    bool isViewReadOnly() const;
    bool isEffectiveReadOnly() const;

    /** returns the index of the cursor position */
    int cursorPosition() const;
    /***/
    bool isCursorBehind() const;

    int noOfBytesPerLine() const;
    int startOffset() const;
    int firstLineOffset() const;

    AbstractByteArrayView::ResizeStyle resizeStyle() const;

    bool tabChangesFocus() const;

    bool hasSelectedData() const;
    KDE::Section selection() const;
    QByteArray selectedData() const;
    QMimeData* selectionAsMimeData() const;

    KController* controller() const;
    AbstractWheelController* wheelController() const;

    KValueEditor* valueEditor() const;
    Dropper* dropper() const;

    ByteArrayTableLayout* layout() const;
    ByteArrayTableCursor* tableCursor() const;
    ByteArrayTableRanges* tableRanges() const;

    const Okteta::ValueCodec* valueCodec() const;
    AbstractByteArrayView::ValueCoding valueCoding() const;
    const Okteta::CharCodec* charCodec() const;
    AbstractByteArrayView::CharCoding charCoding() const;
    const QString& charCodingName() const;

    double zoomLevel() const;

  public:
    void init();
    void setByteArrayModel( Okteta::AbstractByteArrayModel* byteArrayModel );
    // TODO: delete old controller?
    void setController( KController* controller );
    void setWheelController( AbstractWheelController* wheelController );
    void setReadOnly( bool readOnly );
    void setOverwriteOnly( bool overwriteOnly );
    void setOverwriteMode( bool overwriteMode );

  public: // setting parameters
    void setValueCoding( AbstractByteArrayView::ValueCoding valueCoding );
    void setCharCoding( AbstractByteArrayView::CharCoding charCoding );
    void setCharCoding( const QString& charCodingName );
    void setResizeStyle( AbstractByteArrayView::ResizeStyle resizeStyle );
    void setNoOfBytesPerLine( int noOfBytesPerLine );
    void setStartOffset( int startOffset );
    void setFirstLineOffset( int firstLineOffset );
    void setModified( bool modified );

    void setTabChangesFocus( bool tabChangesFocus = true );

  public: // zooming
    void zoomIn( int pointInc );
    void zoomIn();
    void zoomOut( int pointDec );
    void zoomOut();
    void zoomTo( int pointSize );
    void unZoom();
    void setZoomLevel( double level );
    void fontChange( const QFont& oldFont );

  public: // interaction
    void selectAll( bool select );
    bool selectWord( /*unsigned*/ int index /*, Chartype*/ );
    void setSelection( const KDE::Section& selection );

    void setCursorPosition( int index, bool isBehind );
    void setSelectionCursorPosition( int index );

  public: // API to be implemented
    virtual void ensureCursorVisible() = 0;
    virtual void placeCursor( const QPoint& point ) = 0;
    virtual QRect cursorRect() const = 0;

  public: // events
    bool event( QEvent* event );
    void resizeEvent( QResizeEvent* resizeEvent );
    void focusInEvent( QFocusEvent* focusEvent );
    void focusOutEvent( QFocusEvent* focusEvent );
    void dragEnterEvent( QDragEnterEvent* dragEnterEvent );
    void dragMoveEvent( QDragMoveEvent* dragMoveEvent );
    void dragLeaveEvent( QDragLeaveEvent* dragLeaveEvent );
    void dropEvent( QDropEvent* dropEvent );

  public: // slots
    void onContentsChanged( const KDE::ArrayChangeMetricsList& changeList );
    void onBookmarksChange( const QList<Okteta::Bookmark>& bookmarks );
    void onRevertedToVersionIndex( int versionIndex );
    void onByteArrayReadOnlyChange( bool isByteArrayReadOnly );

  protected:
    /** handles screen update in case of a change to any of the width sizes
      */
    void updateViewByWidth();
    void adjustLayoutToSize();
    void adaptController();
    void finishByteEditor();

  protected:
    bool hasChanged( const CoordRange& visibleRange, CoordRange* changedRange ) const;
    bool isByteEditorActive() const;

  protected: // 
    void removeSelectedData();
    void insert( const QByteArray& data );
    void pasteData( const QMimeData* data );
    bool canReadData( const QMimeData* data ) const;

  protected: // clipboard interaction
    void copy();
    void cut();
    void paste();

  protected: // API to be implemented
  // cursor control
    virtual void startCursor() = 0;
    virtual void stopCursor() = 0;
    virtual void pauseCursor() = 0;
    virtual void unpauseCursor() = 0;

    virtual void setActiveCoding( AbstractByteArrayView::CodingTypeId codingId ) = 0;
    virtual void setVisibleCodings( int visibleCodings ) = 0;

    virtual int indexByPoint( const QPoint& point ) const = 0;

  protected: // API to be implemented
    virtual AbstractByteArrayView::CodingTypeId activeCoding() const = 0;
    virtual int visibleCodings() const = 0;
    virtual int fittingBytesPerLine() const = 0;
    /** recalcs all dependant values with the actual NoOfBytesPerLine  */
    virtual void adjustToLayoutNoOfBytesPerLine() = 0;
    /** repaints all the parts that are signed as changed */
    virtual void updateChanged() = 0;

  protected:
    Okteta::AbstractByteArrayModel* mByteArrayModel;

    /** the current input controller */
    KController* mController;
    AbstractWheelController* mWheelController;

    /** holds the logical layout */
    ByteArrayTableLayout* mTableLayout;
    /** */
    ByteArrayTableCursor* mTableCursor;
    /** */
    ByteArrayTableRanges* mTableRanges;

  protected:
    /** */
    KTabController* mTabController;
    /** */
    KNavigator* mNavigator;
    /** */
    KValueEditor* mValueEditor;
    /** */
    KCharEditor* mCharEditor;

    Dropper* mDropper;

    ZoomWheelController* mZoomWheelController;

  protected:
    /** flag whether the widget is set to readonly. Cannot override the databuffer's setting, of course. */
    bool mReadOnly:1;
    /** flag if only overwrite is allowed */
    bool mOverWriteOnly:1;
    /** flag if overwrite mode is active */
    bool mOverWrite:1;
    /** flag whether the font is changed due to a zooming */
    bool mInZooming:1;
    /** flag if the cursor should be invisible */
    bool mCursorPaused:1;

    /** font size as set by user (used for zooming) */
    int mDefaultFontSize;
    double mZoomLevel;
    /** */
    QClipboard::Mode mClipboardMode;

    // parameters
    /** */
    Okteta::ValueCodec* mValueCodec;
    /** */
    AbstractByteArrayView::ValueCoding mValueCoding;
    /** */
    Okteta::CharCodec* mCharCodec;
    /** */
    AbstractByteArrayView::CharCoding mCharCoding;
    /** style of resizing */
    AbstractByteArrayView::ResizeStyle mResizeStyle;

  protected: // 
    AbstractByteArrayView* const q_ptr;
    Q_DECLARE_PUBLIC( AbstractByteArrayView )
};


inline Okteta::AbstractByteArrayModel* AbstractByteArrayViewPrivate::byteArrayModel() const { return mByteArrayModel; }
inline KController* AbstractByteArrayViewPrivate::controller() const { return mController; }
inline AbstractWheelController* AbstractByteArrayViewPrivate::wheelController() const { return mWheelController; }

inline bool AbstractByteArrayViewPrivate::isOverwriteMode() const { return mOverWrite; }
inline bool AbstractByteArrayViewPrivate::isOverwriteOnly() const { return mOverWriteOnly; }
inline bool AbstractByteArrayViewPrivate::isViewReadOnly()      const { return mReadOnly; }
inline bool AbstractByteArrayViewPrivate::isEffectiveReadOnly() const { return mReadOnly || mByteArrayModel->isReadOnly(); }
inline double AbstractByteArrayViewPrivate::zoomLevel()        const { return mZoomLevel; }

inline ByteArrayTableLayout* AbstractByteArrayViewPrivate::layout()      const { return mTableLayout; }
inline ByteArrayTableCursor* AbstractByteArrayViewPrivate::tableCursor() const { return mTableCursor; }
inline ByteArrayTableRanges* AbstractByteArrayViewPrivate::tableRanges() const { return mTableRanges; }

inline const Okteta::ValueCodec* AbstractByteArrayViewPrivate::valueCodec()  const { return mValueCodec; }
inline AbstractByteArrayView::ValueCoding AbstractByteArrayViewPrivate::valueCoding() const { return mValueCoding; }
inline const Okteta::CharCodec* AbstractByteArrayViewPrivate::charCodec()   const { return mCharCodec; }
inline AbstractByteArrayView::CharCoding AbstractByteArrayViewPrivate::charCoding() const
{
    return (AbstractByteArrayView::CharCoding)mCharCoding;
}
inline const QString& AbstractByteArrayViewPrivate::charCodingName() const { return mCharCodec->name(); }
inline AbstractByteArrayView::ResizeStyle AbstractByteArrayViewPrivate::resizeStyle() const { return mResizeStyle; }
inline int AbstractByteArrayViewPrivate::cursorPosition()   const { return mTableCursor->realIndex(); }
inline bool AbstractByteArrayViewPrivate::isCursorBehind()  const { return mTableCursor->isBehind(); }
inline bool AbstractByteArrayViewPrivate::isModified()      const { return mByteArrayModel->isModified(); }
inline int AbstractByteArrayViewPrivate::noOfBytesPerLine() const { return mTableLayout->noOfBytesPerLine(); }
inline int AbstractByteArrayViewPrivate::firstLineOffset()  const { return mTableLayout->firstLineOffset(); }
inline int AbstractByteArrayViewPrivate::startOffset()      const { return mTableLayout->startOffset(); }

inline KDE::Section AbstractByteArrayViewPrivate::selection() const { return mTableRanges->selection(); }
inline bool AbstractByteArrayViewPrivate::hasSelectedData()    const { return mTableRanges->hasSelection(); }

inline bool AbstractByteArrayViewPrivate::tabChangesFocus()      const { return mTabController->tabChangesFocus(); }
inline bool AbstractByteArrayViewPrivate::isByteEditorActive()   const { return mValueEditor->isInEditMode(); }
inline KValueEditor* AbstractByteArrayViewPrivate::valueEditor() const { return mValueEditor; }
inline Dropper* AbstractByteArrayViewPrivate::dropper()          const { return mDropper; }

inline void AbstractByteArrayViewPrivate::finishByteEditor()
{
    mValueEditor->finishEdit();
}
inline void AbstractByteArrayViewPrivate::setTabChangesFocus( bool tabChangesFocus )
{
    mTabController->setTabChangesFocus( tabChangesFocus );
}

inline void AbstractByteArrayViewPrivate::setController( KController* controller ) { mController = controller; }
inline void AbstractByteArrayViewPrivate::setWheelController( AbstractWheelController* wheelController ) { mWheelController = wheelController; }
inline void AbstractByteArrayViewPrivate::setModified( bool modified ) { mByteArrayModel->setModified( modified ); }

}

#endif
