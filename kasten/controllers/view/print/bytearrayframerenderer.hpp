/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYFRAMERENDERER_HPP
#define KASTEN_BYTEARRAYFRAMERENDERER_HPP

// lib
#include "abstractcolumnframerenderer.hpp"
// Okteta gui
#include <Okteta/OffsetFormat>
// Okteta core
#include <Okteta/OktetaCore>
#include <Okteta/Address>
#include <Okteta/Size>
// Qt
#include <QFont>
// Std
#include <memory>

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

enum LayoutStyle
{
    FixedLayoutStyle = 0,
    WrapOnlyByteGroupsLayoutStyle = 1,
    FullSizeLayoutStyle = 2,
    LastUserLayout = 0xFF
};

class ByteArrayFrameRenderer : public AbstractColumnFrameRenderer
{
public:
    enum DataColumnId
    {
        ValueCodingId = 1,
        CharCodingId = 2
    };

public:
    ByteArrayFrameRenderer();
    ~ByteArrayFrameRenderer() override;

public: // AbstractColumnFrameRenderer API
//     virtual void drawColumns( QPainter *painter, int cx, int cy, int cw, int ch );

public: // AbstractFrameRenderer API
    // make this flags?
//     virtual bool hasFixedWidth() const;
//     virtual bool hasFixedHeight() const;
    int height() const override;
    int width() const override;
//     virtual QSize sizeHint( const QSize &maxSize ) const;

    void prepare() override;
    void renderFrame(QPainter* painter, int frameIndex) override;

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
    QString charCodingName() const;

    bool offsetColumnVisible() const;
    Okteta::OffsetFormat::Format offsetCoding() const;

    int visibleByteArrayCodings() const;

public:
    void setByteArrayModel(Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset = 0, Okteta::Size length = -1);
    void setHeight(int height);
    void setWidth(int width);
    void setFont(const QFont& font);
    void setFirstLineOffset(Okteta::Address firstLineOffset);
    void setStartOffset(Okteta::Address startOffset);
    void setBufferSpacing(Okteta::PixelX byteSpacing, int noOfGroupedBytes, Okteta::PixelX groupSpacing);
    void setValueCoding(Okteta::ValueCoding valueCoding);
    void setLayoutStyle(LayoutStyle style);
    void setNoOfBytesPerLine(int noOfBytesPerLine);
    void setByteSpacingWidth(Okteta::PixelX byteSpacingWidth);
    void setNoOfGroupedBytes(int noOfGroupedBytes);
    void setGroupSpacingWidth(Okteta::PixelX groupSpacingWidth);
    void setBinaryGapWidth(Okteta::PixelX binaryGapWidth);
    void setSubstituteChar(QChar substituteChar);
    void setUndefinedChar(QChar undefinedChar);
    void setShowsNonprinting(bool showsNonprinting);
    void setCharCoding(const QString& charCodingName);
    void showByteArrayColumns(int newColumns);
    void showOffsetColumn(bool visible);
    void setOffsetCoding(Okteta::OffsetFormat::Format offsetCoding);

protected: // AbstractColumnFrameRenderer API
//     virtual void setNoOfLines( int newNoOfLines );

private:
    void adjustToWidth();
    void adjustLayoutToSize();
    void adjustToLayoutNoOfBytesPerLine();

private:
    int fittingBytesPerLine() const;

private:
    int mHeight;
    int mWidth;
    QFont mFont;

    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;

    // in (reverse) order of destruction
private:
    /** holds the logical layout */
    std::unique_ptr<Okteta::ByteArrayTableLayout> mLayout;
    std::unique_ptr<Okteta::ByteArrayTableRanges> mTableRanges;

private:
    std::unique_ptr<Okteta::PrintColumnStylist> mStylist;
    Okteta::OffsetColumnRenderer*         mOffsetColumnRenderer;
    Okteta::BorderColumnRenderer*         mFirstBorderColumnRenderer;
    Okteta::ValueByteArrayColumnRenderer* mValueColumnRenderer;
    Okteta::BorderColumnRenderer*         mSecondBorderColumnRenderer;
    Okteta::CharByteArrayColumnRenderer*  mCharColumnRenderer;

private:
    /** */
    std::unique_ptr<const Okteta::ValueCodec> mValueCodec;
    /** */
    Okteta::ValueCoding mValueCoding;
    /** */
    std::unique_ptr<const Okteta::CharCodec> mCharCodec;

private: // parameters
    LayoutStyle mResizeStyle;
};

#endif
