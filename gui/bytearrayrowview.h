/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2008-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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
class ByteArrayRowViewPrivate;


class OKTETAGUI_EXPORT ByteArrayRowView : public AbstractByteArrayView
{
  Q_OBJECT

  public:
    explicit ByteArrayRowView( QWidget* parent = 0 );
    virtual ~ByteArrayRowView();

  public: // AbstractByteArrayView API
  // value column
    virtual int/*PixelX*/ byteSpacingWidth() const;
    virtual int noOfGroupedBytes() const;
    virtual int/*PixelX*/ groupSpacingWidth() const;
    virtual int/*PixelX*/ binaryGapWidth() const;

  // char column
    virtual bool showsNonprinting() const;
    virtual QChar substituteChar() const;
    virtual QChar undefinedChar() const;

    virtual bool isByteTypeColored() const;

    virtual void setByteArrayModel( AbstractByteArrayModel* byteArrayModel );
    virtual void setByteSpacingWidth( int/*PixelX*/ byteSpacingWidth ) ;
    virtual void setNoOfGroupedBytes( int noOfGroupedBytes );
    virtual void setGroupSpacingWidth( int/*PixelX*/ groupSpacingWidth );
    virtual void setBinaryGapWidth( int binaryGapWidth );
    virtual void setBufferSpacing( int/*PixelX*/ byteSpacingWidth, int noOfGroupedBytes = 0, int/*PixelX*/ groupSpacingWidth = 0 );
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

  public: // modification access
//    void repaintByte( int row, int column, bool Erase = true );
//    void updateByte( int row, int column );
//    void ensureByteVisible( int row, int column );

  protected: // Q_SLOTS QWidget API
    virtual void changeEvent( QEvent* event );

  private:
    Q_DECLARE_PRIVATE( ByteArrayRowView )
};

}

#endif
