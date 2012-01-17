/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYVIEW_H
#define BYTEARRAYVIEW_H

// lib
#include "oktetakastengui_export.h"
// lib
#include <bytearrayselection.h>
// Kasten gui
#include <abstractview.h>
#include <zoomable.h> // TODO: able instead of interface? or else?
#include <dataselectable.h>
#include <selecteddatawriteable.h>
// Okteta core
#include <addressrange.h>

namespace Okteta {
class ByteArrayJanusView;
}
class QRect;


namespace Kasten2
{
class ByteArrayViewProfileSynchronizer;
class ByteArrayDocument;
class KCursorProxy;


class OKTETAKASTENGUI_EXPORT ByteArrayView : public AbstractView,
                                             public If::Zoomable,
                                             public If::DataSelectable,
                                             public If::SelectedDataWriteable
{
  Q_OBJECT
  Q_INTERFACES(
    Kasten2::If::Zoomable
    Kasten2::If::DataSelectable
    Kasten2::If::SelectedDataWriteable
  )

  public:
    enum { ColumnViewId = 0, RowViewId };

  public:
    explicit ByteArrayView( ByteArrayDocument* document );
    explicit ByteArrayView( ByteArrayView* other, Qt::Alignment alignment = 0 );

    virtual ~ByteArrayView();

  public:
//     KCursorObject *cursor() const;

  public: // AbstractModel API
    virtual QString title() const;
    virtual bool isModifiable() const;
    virtual bool isReadOnly() const;
    virtual void setReadOnly( bool isReadOnly );

  public: // AbstractView API
    virtual void setFocus();
    virtual QWidget* widget() const;
    virtual bool hasFocus() const;

  public: // If::Zoomable API TODO: make all such methods slots?
    virtual void setZoomLevel( double Level );
    virtual double zoomLevel() const;
  Q_SIGNALS:
    /*virtual*/ void zoomLevelChanged( double Level );

  public: // If::DataSelectable API
    virtual void selectAllData( bool selectAll );
    virtual bool hasSelectedData() const;
    virtual QMimeData* copySelectedData() const;
    virtual const AbstractModelSelection* modelSelection() const;
//     virtual void setSelection();
  Q_SIGNALS:
    /*virtual*/ void hasSelectedDataChanged( bool hasSelectedData );
    /*virtual*/ void selectedDataChanged( const Kasten2::AbstractModelSelection* modelSelection );

  public: // If::SelectedDataWriteable API
    virtual void insertData( const QMimeData* data );
    virtual QMimeData* cutSelectedData();
    virtual void deleteSelectedData();
    virtual bool canReadData( const QMimeData* data ) const;

  public: // cursor API
    void setCursorPosition( Okteta::Address cursorPosition );
    void setSelectionCursorPosition( Okteta::Address index );
    Okteta::Address cursorPosition() const;
  Q_SIGNALS:
    void cursorPositionChanged( Okteta::Address cursorPosition );

  public: // codings
    void setValueCoding( int valueCoding );
    void setCharCoding( const QString& charCodingName );
    QString charCodingName() const;
    int valueCoding() const;
  Q_SIGNALS:
    void charCodecChanged( const QString& charCodingName );
    void valueCodingChanged( int valueCoding );

  public:
    // TODO: see how this can be solved by modelSelection
    Okteta::AddressRange selection() const;
    void setSelection( Okteta::Address start, Okteta::Address end );
    void insert( const QByteArray& byteArray );
    QRect cursorRect() const;

  public: // overwrite
    void setOverwriteMode( bool overwriteMode );
    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;
  Q_SIGNALS:
    void overwriteModeChanged( bool overwriteMode );

  public: // elements
    void toggleOffsetColumn( bool visible );
    void setVisibleByteArrayCodings( int columns );
    void setMarking( const Okteta::AddressRange& range, bool ensureVisible = false );
    bool offsetColumnVisible() const;
    int visibleByteArrayCodings() const;
  Q_SIGNALS:
    void offsetColumnVisibleChanged( bool visible );
    void visibleByteArrayCodingsChanged( int columns );

  public: // table layout
    void setLayoutStyle( int layoutStyle );
    void setNoOfBytesPerLine( int noOfBytesPerLine );
    Okteta::Address startOffset() const;
    Okteta::Address firstLineOffset() const;
    int noOfBytesPerLine() const;
    int layoutStyle() const;
  Q_SIGNALS:
    void layoutStyleChanged( int layoutStyle );
    void noOfBytesPerLineChanged( int noOfBytesPerLine );

  public: // layout settings
    void setShowsNonprinting( bool showsNonprinting = true );
    void setNoOfGroupedBytes( int noOfGroupedBytes );
    void setSubstituteChar( QChar substituteChar );
    void setUndefinedChar( QChar undefinedChar );
    QChar substituteChar() const;
    QChar undefinedChar() const;
    bool showsNonprinting() const;

    int byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    int groupSpacingWidth() const;
    int binaryGapWidth() const;
  Q_SIGNALS:
    void showsNonprintingChanged( bool showsNonprinting );
    void substituteCharChanged( QChar substituteChar );
    void undefinedCharChanged( QChar undefinedChar );
    void noOfGroupedBytesChanged( int noOfGroupedBytes );

  public:
    void setViewModus( int viewModus );
    int viewModus() const;
  Q_SIGNALS:
    void viewModusChanged( int viewModus );

  public:
    void setSynchronizer( ByteArrayViewProfileSynchronizer* synchronizer );
    ByteArrayViewProfileSynchronizer* synchronizer() const;

  private:
    void init();

  private Q_SLOTS:
    void setFontByGlobalSettings();

    void onSelectionChanged( const Okteta::AddressRange& selection );

  private:
    Okteta::ByteArrayJanusView* mWidget;
    ByteArrayDocument* mDocument;
    ByteArraySelection mSelection;
//     KCursorProxy *mCursorProxy;

    ByteArrayViewProfileSynchronizer* mByteArrayViewProfileSynchronizer;
};

}

#endif
