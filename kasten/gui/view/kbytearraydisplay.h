/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KBYTEARRAYDISPLAY_H
#define KBYTEARRAYDISPLAY_H

// lib
#include "oktetakastengui_export.h"
// lib
#include <kbytearrayselection.h>
// Kasten gui
#include <abstractview.h>
#include <zoomable.h> // TODO: able instead of interface? or else?
#include <dataselectable.h>
#include <selecteddatawriteable.h>

namespace Okteta {
class ByteArrayJanusView;
}
namespace KDE {
class Section;
}
class QRect;


namespace Kasten
{

class KByteArrayDocument;
class KCursorProxy;


class OKTETAKASTENGUI_EXPORT KByteArrayDisplay : public AbstractView, public If::Zoomable, public If::DataSelectable, public If::SelectedDataWriteable
{
    Q_OBJECT
    Q_INTERFACES(Kasten::If::Zoomable Kasten::If::DataSelectable Kasten::If::SelectedDataWriteable)

  public:
    enum { ColumnViewId = 0, RowViewId };

  public:
    explicit KByteArrayDisplay( KByteArrayDocument* document );

    virtual ~KByteArrayDisplay();

  public:
//     KCursorObject *cursor() const;

  public: // AbstractModel API
    virtual QString title() const;
    virtual bool isModifiable() const;
    virtual bool isReadOnly() const;
    virtual void setReadOnly( bool isReadOnly );

  public: // AbstractView API
    virtual QWidget* widget() const;

  public: // If::Zoomable API TODO: make all such methods slots?
    virtual void setZoomLevel( double Level );
    virtual double zoomLevel() const;
  Q_SIGNALS:
    virtual void zoomLevelChanged( double Level );

  public: // If::DataSelectable API
    virtual void selectAllData( bool selectAll );
    virtual bool hasSelectedData() const;
    virtual QMimeData *copySelectedData() const;
    virtual const AbstractModelSelection* modelSelection() const;
//     virtual void setSelection();
  Q_SIGNALS:
    virtual void hasSelectedDataChanged( bool hasSelectedData );

  public: // If::SelectedDataWriteable API
    virtual void insertData( const QMimeData *data );
    virtual QMimeData *cutSelectedData();
    virtual void deleteSelectedData();
    virtual bool canReadData( const QMimeData *data ) const;

  public: // cursor API
    void setCursorPosition( int cursorPosition );
    void setSelectionCursorPosition( int index );
    int cursorPosition() const;
  Q_SIGNALS:
    void cursorPositionChanged( int cursorPosition );

  public: // codings
    void setValueCoding( int valueCoding );
    void setCharCoding( const QString& charCodingName );
    QString charCodingName() const;
    int valueCoding() const;
  Q_SIGNALS:
    void charCodecChanged( const QString& charCodingName );
    void valueCodingChanged( int valueCoding );

  public:
    void setFocus();
    KDE::Section selection() const;
    void setSelection( int start, int end );
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
    bool offsetColumnVisible() const;
    int visibleByteArrayCodings() const;

  public: // table layout
    void setResizeStyle( int resizeStyle );
    int startOffset() const;
    int firstLineOffset() const;
    int noOfBytesPerLine() const;
    int resizeStyle() const;

  public: // layout settings
    void setShowsNonprinting( bool showsNonprinting = true );
    QChar substituteChar() const;
    QChar undefinedChar() const;
    bool showsNonprinting() const;

    int byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    int groupSpacingWidth() const;
    int binaryGapWidth() const;

  public:
    void setViewModus( int viewModus );
    int viewModus() const;

  protected Q_SLOTS:
    void onSelectionChange( bool selected );

  protected:
    Okteta::ByteArrayJanusView* mWidget;
    KByteArrayDocument* mDocument;
    KByteArraySelection mSelection;
//     KCursorProxy *mCursorProxy;
};

}

#endif
