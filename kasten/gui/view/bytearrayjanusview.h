/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#ifndef BYTEARRAYJANUSVIEW_H
#define BYTEARRAYJANUSVIEW_H

// Qt
#include <QtGui/QWidget>

namespace Okteta {
class AbstractByteArrayModel;
}
namespace KDE {
class Section;
}
class QHBoxLayout;
class QMimeData;

namespace Okteta
{
class AbstractByteArrayView;

class ByteArrayJanusView : public QWidget
{
  Q_OBJECT

  public:
    enum { ColumnViewId = 0, RowViewId };

  public:
    explicit ByteArrayJanusView( QWidget* parent = 0 );
    virtual ~ByteArrayJanusView();

  public:
    void setByteArrayModel( Okteta::AbstractByteArrayModel* byteArrayModel );

    void setZoomLevel( double Level );
    double zoomLevel() const;

  public: // 
    void setReadOnly( bool isReadOnly );
    bool isReadOnly() const;

  public: // cursor API
    void setCursorPosition( int cursorPosition );
    void setSelectionCursorPosition( int index );
    int cursorPosition() const;
    QRect cursorRect() const;

  public: // codings
    void setValueCoding( int valueCoding );
    void setCharCoding( const QString& charCodingName );
    QString charCodingName() const;
    int valueCoding() const;

  public:
    void setSelection( int start, int end );
    void selectAll( bool selectAll );
    void removeSelectedData();
    void pasteData( const QMimeData* data );
    void insert( const QByteArray& byteArray );
    bool hasSelectedData() const;
    QMimeData* selectionAsMimeData() const;
    bool canReadData( const QMimeData* data ) const;
    KDE::Section selection() const;

  public: // overwrite
    void setOverwriteMode( bool overwriteMode );
    bool isOverwriteMode() const;
    bool isOverwriteOnly() const;

  public: // elements
    void toggleOffsetColumn( bool visible );
    void setVisibleCodings( int columns );
    bool offsetColumnVisible() const;
    int visibleCodings() const;

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

  Q_SIGNALS:
    void readOnlyChanged( bool );
    void overwriteModeChanged( bool overwriteMode );
    void selectionChanged( bool hasSelectedData );
    void cursorPositionChanged( int cursorPosition );
    void valueCodingChanged( int valueCoding );
    void charCodecChanged( const QString& charCodingName );
    void focusChanged( bool hasFocus );

  private:
    AbstractByteArrayView* mView;
    int mViewModus;
    QHBoxLayout* mLayout;
};

inline int ByteArrayJanusView::viewModus()               const { return mViewModus; }

}

#endif
