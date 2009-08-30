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

#ifndef OKTETA_BYTEARRAYROWVIEW_H
#define OKTETA_BYTEARRAYROWVIEW_H

// lib
#include "abstractbytearrayview.h"


namespace Okteta
{
class KValueEditor;

class ByteArrayRowViewPrivate;

class OKTETAGUI_EXPORT ByteArrayRowView : public AbstractByteArrayView
{
  Q_OBJECT
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
    explicit ByteArrayRowView( QWidget* parent = 0 );
    virtual ~ByteArrayRowView();

  public: // AbstractByteArrayView API
    virtual bool offsetColumnVisible() const;

  // value column
    virtual int/*KPixelX*/ byteSpacingWidth() const;
    virtual int noOfGroupedBytes() const;
    virtual int/*KPixelX*/ groupSpacingWidth() const;
    virtual int/*KPixelX*/ binaryGapWidth() const;

  // char column
    virtual bool showsNonprinting() const;
    virtual QChar substituteChar() const;
    virtual QChar undefinedChar() const;

    virtual bool isByteTypeColored() const;

    virtual void setByteArrayModel( AbstractByteArrayModel* byteArrayModel );
    virtual void toggleOffsetColumn( bool offsetColumnVisible );
    virtual void setByteSpacingWidth( int/*KPixelX*/ byteSpacingWidth ) ;
    virtual void setNoOfGroupedBytes( int noOfGroupedBytes );
    virtual void setGroupSpacingWidth( int/*KPixelX*/ groupSpacingWidth );
    virtual void setBinaryGapWidth( int binaryGapWidth );
    virtual void setBufferSpacing( int/*KPixelX*/ byteSpacingWidth, int noOfGroupedBytes = 0, int/*KPixelX*/ groupSpacingWidth = 0 );
    virtual void setValueCoding( ValueCoding valueCoding );
  // char column parameters
    virtual void setShowsNonprinting( bool showsNonprinting = true );
    virtual void setSubstituteChar( QChar substituteChar );
    virtual void setUndefinedChar( QChar undefinedChar );
    virtual void setCharCoding( CharCoding charCoding );
    virtual void setCharCoding( const QString& charCodingName );
    virtual void setByteTypeColored( bool isColored );

  public: // ColumnsView API
    virtual void renderColumns( QPainter* painter, int cx, int cy, int cw, int ch );

  public: // QWidget API
    virtual QSize minimumSizeHint() const;

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
    Address indexByPoint(const QPoint& point ) const;

  public: // modification access
//    void repaintByte( int row, int column, bool Erase = true );
//    void updateByte( int row, int column );
//    void ensureByteVisible( int row, int column );

  protected: // QAbstractScrollArea API
    virtual void mousePressEvent( QMouseEvent* mousePressEvent );
    virtual void mouseReleaseEvent( QMouseEvent* mouseReleaseEvent );
    virtual void mouseMoveEvent( QMouseEvent* mouseMoveEvent );
    virtual void mouseDoubleClickEvent( QMouseEvent* mouseDoubleClickEvent );
//    virtual void contextMenuEvent( QContextMenuEvent* contextMenuEvent );

  protected:
    void updateCursors();

  protected: // Q_SLOTS QWidget API
    virtual void fontChange( const QFont& oldFont );

  private:
    Q_DECLARE_PRIVATE( ByteArrayRowView )
    Q_PRIVATE_SLOT( d_func(), void blinkCursor() )
    Q_PRIVATE_SLOT( d_func(), void autoScrollTimerDone() )
//     Q_PRIVATE_SLOT( d_func(), void clipboardChanged() )
    Q_PRIVATE_SLOT( d_func(), void startDrag() )
};

}

#endif
