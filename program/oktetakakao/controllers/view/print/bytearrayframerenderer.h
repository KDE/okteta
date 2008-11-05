/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <khe.h>
// Qt
#include <QtGui/QFont>


namespace KHEPrint {
class OffsetColumnRenderer;
class BorderColumnRenderer;
class ValueColumnRenderer;
class CharColumnRenderer;
}
namespace KHEUI {
class ByteArrayTableLayout;
}
namespace KHECore {
class AbstractByteArrayModel;
class CharCodec;
}

enum ResizeStyle { NoResize=0, LockGrouping=1, FullSizeUsage=2, MaxResizeStyleId=0xFF };

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
    virtual void renderFrame( QPainter *painter, int frameIndex );

  public: // AbstractSerialFramePrinter
    // only vertical for now...
    virtual int framesCount() const;

  public:
    const KHECore::AbstractByteArrayModel *byteArrayModel() const;
    int offset() const;
    int length() const;

    int noOfBytesPerLine() const;
    int firstLineOffset() const;
    int startOffset() const;
    ResizeStyle resizeStyle() const;
    KHECore::ValueCoding valueCoding() const;
    KPixelX byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    KPixelX groupSpacingWidth() const;
    KPixelX binaryGapWidth() const;
    bool showsNonprinting() const;
    QChar substituteChar() const;
    QChar undefinedChar() const;
    KHECore::CharCoding charCoding() const;
    const QString &charCodingName() const;

    bool offsetColumnVisible() const;
    int visibleByteArrayCodings() const;

  public:
    void setByteArrayModel( const KHECore::AbstractByteArrayModel *byteArrayModel, int offset = 0, int length = -1 );
    void setHeight( int height );
    void setWidth( int width );
    void setFont( const QFont &font );
    void setFirstLineOffset( int firstLineOffset );
    void setStartOffset( int startOffset );
    void setBufferSpacing( KPixelX byteSpacing, int noOfGroupedBytes, KPixelX groupSpacing );
    void setValueCoding( KHECore::ValueCoding valueCoding );
    void setResizeStyle( ResizeStyle style );
    void setNoOfBytesPerLine( int noOfBytesPerLine );
    void setByteSpacingWidth( KPixelX byteSpacingWidth );
    void setNoOfGroupedBytes( int noOfGroupedBytes );
    void setGroupSpacingWidth( KPixelX groupSpacingWidth );
    void setBinaryGapWidth( KPixelX binaryGapWidth );
    void setSubstituteChar( QChar substituteChar );
    void setUndefinedChar( QChar undefinedChar );
    void setShowsNonprinting( bool showsNonprinting );
    void setCharCoding( KHECore::CharCoding charCoding );
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

    const KHECore::AbstractByteArrayModel *mByteArrayModel;

  protected:
    /** holds the logical layout */
    KHEUI::ByteArrayTableLayout *mLayout;

  protected:
    KHEPrint::OffsetColumnRenderer *mOffsetColumnRenderer;
    KHEPrint::BorderColumnRenderer *mFirstBorderColumnRenderer;
    KHEPrint::ValueColumnRenderer  *mValueColumnRenderer;
    KHEPrint::BorderColumnRenderer *mSecondBorderColumnRenderer;
    KHEPrint::CharColumnRenderer   *mCharColumnRenderer;

  protected:
    /** */
    KHECore::CharCodec *mCodec;

  protected: // parameters
    ResizeStyle mResizeStyle;
    /** */
    KHECore::CharCoding mEncoding;
};

#endif
