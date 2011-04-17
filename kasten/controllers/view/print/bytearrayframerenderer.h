/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYFRAMERENDERER_H
#define BYTEARRAYFRAMERENDERER_H

// lib
#include "abstractcolumnframerenderer.h"
// Okteta core
#include <oktetacore.h>
#include <address.h>
#include <size.h>
// Qt
#include <QtGui/QFont>

namespace Okteta {
class OffsetColumnRenderer;
class BorderColumnRenderer;
class ValueByteArrayColumnRenderer;
class CharByteArrayColumnRenderer;
class ByteArrayTableLayout;
class ByteArrayTableRanges;
class PrintColumnStylist;
}
namespace Okteta {
class AbstractByteArrayModel;
class ValueCodec;
class CharCodec;
}


enum LayoutStyle { FixedLayoutStyle=0, WrapOnlyByteGroupsLayoutStyle=1, FullSizeLayoutStyle=2, LastUserLayout=0xFF };


class ByteArrayFrameRenderer : public AbstractColumnFrameRenderer
{
  public:
    enum KDataColumnId { ValueCodingId=1, CharCodingId=2 };

  public:
    ByteArrayFrameRenderer();
    virtual ~ByteArrayFrameRenderer();

  public: // AbstractColumnFrameRenderer API
//     virtual void drawColumns( QPainter *painter, int cx, int cy, int cw, int ch );

  public: // AbstractFrameRenderer API
    // make this flags?
//     virtual bool hasFixedWidth() const;
//     virtual bool hasFixedHeight() const;
    virtual int height() const;
    virtual int width() const;
//     virtual QSize sizeHint( const QSize &maxSize ) const;

    virtual void prepare();
    virtual void renderFrame( QPainter* painter, int frameIndex );

  public: // AbstractSerialFramePrinter
    // only vertical for now...
    virtual int framesCount() const;

  public:
    Okteta::AbstractByteArrayModel* byteArrayModel() const;
    Okteta::Address offset() const;
    Okteta::Size length() const;

    int noOfBytesPerLine() const;
    Okteta::Address firstLineOffset() const;
    Okteta::Address startOffset() const;
    LayoutStyle layoutStyle() const;
    Okteta::ValueCoding valueCoding() const;
    Okteta::PixelX byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    Okteta::PixelX groupSpacingWidth() const;
    Okteta::PixelX binaryGapWidth() const;
    bool showsNonprinting() const;
    QChar substituteChar() const;
    QChar undefinedChar() const;
    Okteta::CharCoding charCoding() const;
    const QString& charCodingName() const;

    bool offsetColumnVisible() const;
    int visibleByteArrayCodings() const;

  public:
    void setByteArrayModel( Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset = 0, Okteta::Size length = -1 );
    void setHeight( int height );
    void setWidth( int width );
    void setFont( const QFont& font );
    void setFirstLineOffset( Okteta::Address firstLineOffset );
    void setStartOffset( Okteta::Address startOffset );
    void setBufferSpacing( Okteta::PixelX byteSpacing, int noOfGroupedBytes, Okteta::PixelX groupSpacing );
    void setValueCoding( Okteta::ValueCoding valueCoding );
    void setLayoutStyle( LayoutStyle style );
    void setNoOfBytesPerLine( int noOfBytesPerLine );
    void setByteSpacingWidth( Okteta::PixelX byteSpacingWidth );
    void setNoOfGroupedBytes( int noOfGroupedBytes );
    void setGroupSpacingWidth( Okteta::PixelX groupSpacingWidth );
    void setBinaryGapWidth( Okteta::PixelX binaryGapWidth );
    void setSubstituteChar( QChar substituteChar );
    void setUndefinedChar( QChar undefinedChar );
    void setShowsNonprinting( bool showsNonprinting );
    void setCharCoding( Okteta::CharCoding charCoding );
    void setCharCoding( const QString &charCodingName );
    void showByteArrayColumns( int CCs );
    void showOffsetColumn( bool visible );

  protected: // AbstractColumnFrameRenderer API
//     virtual void setNoOfLines( int newNoOfLines );

  protected:
    void adjustToWidth();
    void adjustLayoutToSize();
    void adjustToLayoutNoOfBytesPerLine();


  protected:
    int fittingBytesPerLine() const;

  protected:
    int mHeight;
    int mWidth;
    QFont mFont;

    Okteta::AbstractByteArrayModel* mByteArrayModel;

  protected:
    /** holds the logical layout */
    Okteta::ByteArrayTableLayout* mLayout;
    Okteta::ByteArrayTableRanges* mTableRanges;

  protected:
    Okteta::OffsetColumnRenderer*         mOffsetColumnRenderer;
    Okteta::BorderColumnRenderer*         mFirstBorderColumnRenderer;
    Okteta::ValueByteArrayColumnRenderer* mValueColumnRenderer;
    Okteta::BorderColumnRenderer*         mSecondBorderColumnRenderer;
    Okteta::CharByteArrayColumnRenderer*  mCharColumnRenderer;
    Okteta::PrintColumnStylist* mStylist;

  protected:
    /** */
    Okteta::ValueCodec* mValueCodec;
    /** */
    Okteta::ValueCoding mValueCoding;
    /** */
    Okteta::CharCodec* mCharCodec;
    /** */
    Okteta::CharCoding mCharCoding;

  protected: // parameters
    LayoutStyle mResizeStyle;
};

#endif
