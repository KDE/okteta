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


class OffsetColumnRenderer;
class BorderColumnRenderer;
class ValueColumnRenderer;
class CharColumnRenderer;

namespace KHEUI {
class ByteArrayTableLayout;
}
namespace KHECore {
class KAbstractByteArrayModel;
class KCharCodec;
}

enum KResizeStyle { NoResize=0, LockGrouping=1, FullSizeUsage=2, MaxResizeStyleId=0xFF };

class ByteArrayFrameRenderer : public AbstractColumnFrameRenderer
{
  public:
    enum KDataColumnId { ValueColumnId=1, CharColumnId=2 };

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
    const KHECore::KAbstractByteArrayModel *byteArrayModel() const;
    int offset() const;
    int length() const;

    int noOfBytesPerLine() const;
    int firstLineOffset() const;
    int startOffset() const;
    KResizeStyle resizeStyle() const;
    KHECore::KCoding coding() const;
    KPixelX byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    KPixelX groupSpacingWidth() const;
    KPixelX binaryGapWidth() const;
    bool showsNonprinting() const;
    QChar substituteChar() const;
    QChar undefinedChar() const;
    KHECore::KEncoding encoding() const;
    const QString &encodingName() const;

    bool offsetColumnVisible() const;
    int visibleByteArrayColumns() const;

  public:
    void setByteArrayModel( const KHECore::KAbstractByteArrayModel *byteArrayModel, int offset = 0, int length = -1 );
    void setHeight( int height );
    void setWidth( int width );
    void setFont( const QFont &font );
    void setFirstLineOffset( int firstLineOffset );
    void setStartOffset( int startOffset );
    void setBufferSpacing( KPixelX byteSpacing, int noOfGroupedBytes, KPixelX groupSpacing );
    void setCoding( KHECore::KCoding coding );
    void setResizeStyle( KResizeStyle style );
    void setNoOfBytesPerLine( int noOfBytesPerLine );
    void setByteSpacingWidth( KPixelX byteSpacingWidth );
    void setNoOfGroupedBytes( int noOfGroupedBytes );
    void setGroupSpacingWidth( KPixelX groupSpacingWidth );
    void setBinaryGapWidth( KPixelX binaryGapWidth );
    void setSubstituteChar( QChar substituteChar );
    void setUndefinedChar( QChar undefinedChar );
    void setShowsNonprinting( bool showsNonprinting );
    void setEncoding( KHECore::KEncoding encoding );
    void setEncoding( const QString &encodingName );
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

    const KHECore::KAbstractByteArrayModel *mByteArrayModel;

  protected:
    /** holds the logical layout */
    KHEUI::ByteArrayTableLayout *mLayout;

  protected:
    OffsetColumnRenderer *mOffsetColumnRenderer;
    BorderColumnRenderer *mFirstBorderColumnRenderer;
    ValueColumnRenderer  *mValueColumnRenderer;
    BorderColumnRenderer *mSecondBorderColumnRenderer;
    CharColumnRenderer   *mCharColumnRenderer;

  protected:
    /** */
    KHECore::KCharCodec *mCodec;

  protected: // parameters
    KResizeStyle mResizeStyle;
    /** */
    KHECore::KEncoding mEncoding;
};

#endif
