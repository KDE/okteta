/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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
#include <kbytearrayselection.h>
// Kakao gui
#include <kabstractview.h>
#include <kizoomable.h> // TODO: able instead of interface? or else?
#include <kidataselectable.h>
#include <kiselecteddatawriteable.h>

namespace KHEUI {
class ByteArrayJanusView;
}
class KByteArrayDocument;
class KCursorProxy;
namespace KHE {
class KSection;
}

class KByteArrayDisplay : public KAbstractView, public KDE::If::Zoomable, public KDE::If::DataSelectable, public KDE::If::SelectedDataWriteable
{
    Q_OBJECT
    Q_INTERFACES(KDE::If::Zoomable KDE::If::DataSelectable KDE::If::SelectedDataWriteable)

  public:
    enum { ColumnViewId = 0, RowViewId };

  public:
    KByteArrayDisplay( KByteArrayDocument *Document );
    virtual ~KByteArrayDisplay();

  public:
//     KCursorObject *cursor() const;

  public: // AbstractModel API
    virtual QString title() const;
    virtual bool isModifiable() const;
    virtual bool isReadOnly() const;
    virtual void setReadOnly( bool isReadOnly );

  public: // KAbstractView API
    virtual QWidget *widget() const;
    virtual KAbstractDocument *document() const;

  public: // KDE::If::Zoomable API TODO: make all such methods slots?
    virtual void setZoomLevel( double Level );
    virtual double zoomLevel() const;
  Q_SIGNALS:
    virtual void zoomLevelChanged( double Level );

  public: // KDE::If::DataSelectable API
    virtual void selectAllData( bool selectAll );
    virtual bool hasSelectedData() const;
    virtual QMimeData *copySelectedData() const;
    virtual const AbstractModelSelection* modelSelection() const;
//     virtual void setSelection();
  Q_SIGNALS:
    virtual void hasSelectedDataChanged( bool hasSelectedData );

  public: // KDE::If::SelectedDataWriteable API
    virtual void insertData( const QMimeData *data );
    virtual QMimeData *cutSelectedData();
    virtual void deleteSelectedData();
    virtual bool canReadData( const QMimeData *data ) const;

  public: // cursor API
    void setCursorPosition( int cursorPosition );
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
    KHE::KSection selection() const;
    void setSelection( int start, int end );
    void insert( const QByteArray& byteArray );

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
    KHEUI::ByteArrayJanusView* mWidget;
    KByteArrayDocument* mDocument;
    KByteArraySelection mSelection;
//     KCursorProxy *mCursorProxy;
};

#endif
