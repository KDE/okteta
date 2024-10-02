/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYFRAMERENDERER_HPP
#define KASTEN_ABSTRACTBYTEARRAYFRAMERENDERER_HPP

// lib
#include "abstractcolumnframerenderer.hpp"
// Okteta Gui
#include <Okteta/OffsetFormat>
// Okteta Core
#include <Okteta/OktetaCore>
#include <Okteta/Address>
#include <Okteta/Size>
// Qt
#include <QFont>
// Std
#include <memory>

namespace Okteta {
class AbstractByteArrayModel;
class ByteArrayTableLayout;
class ByteArrayTableRanges;

class PrintColumnStylist;

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

class AbstractByteArrayFrameRenderer : public AbstractColumnFrameRenderer
{
public:
    enum DataColumnId
    {
        ValueCodingId = 1,
        CharCodingId = 2
    };

public:
    AbstractByteArrayFrameRenderer();
    ~AbstractByteArrayFrameRenderer() override;

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

public: // API to implement
    virtual void setByteArrayModel(Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset = 0, Okteta::Size length = -1) = 0;
    virtual void setFont(const QFont& font) = 0;
    virtual void setBufferSpacing(Okteta::PixelX byteSpacing, int noOfGroupedBytes, Okteta::PixelX groupSpacing) = 0;
    virtual void setValueCoding(Okteta::ValueCoding valueCoding) = 0;
    virtual void setByteSpacingWidth(Okteta::PixelX byteSpacingWidth) = 0;
    virtual void setNoOfGroupedBytes(int noOfGroupedBytes) = 0;
    virtual void setGroupSpacingWidth(Okteta::PixelX groupSpacingWidth) = 0;
    virtual void setBinaryGapWidth(Okteta::PixelX binaryGapWidth) = 0;
    virtual void setSubstituteChar(QChar substituteChar) = 0;
    virtual void setUndefinedChar(QChar undefinedChar) = 0;
    virtual void setShowsNonprinting(bool showsNonprinting) = 0;
    virtual void setCharCoding(const QString& charCodingName) = 0;
    virtual void showByteArrayColumns(int CCs) = 0;
    virtual void showOffsetColumn(bool visible) = 0;
    virtual void setOffsetCoding(Okteta::OffsetFormat::Format offsetCoding) = 0;

public:
    Okteta::AbstractByteArrayModel* byteArrayModel() const;
    Okteta::Address offset() const;
    Okteta::Size length() const;

    int noOfBytesPerLine() const;
    Okteta::Address firstLineOffset() const;
    Okteta::Address startOffset() const;
    LayoutStyle layoutStyle() const;
    Okteta::ValueCoding valueCoding() const;
    QString charCodingName() const;

public:
    void setHeight(int height);
    void setWidth(int width);
    void setFirstLineOffset(Okteta::Address firstLineOffset);
    void setStartOffset(Okteta::Address startOffset);
    void setLayoutStyle(LayoutStyle style);
    void setNoOfBytesPerLine(int noOfBytesPerLine);

protected:
    void adjustToWidth();
    void adjustLayoutToSize();

protected:
    virtual void adjustToLayoutNoOfBytesPerLine() = 0;

    virtual int fittingBytesPerLine() const = 0;

protected:
    int mHeight;
    int mWidth;
    QFont mFont;

    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;

    // in (reverse) order of destruction
protected:
    /** holds the logical layout */
    std::unique_ptr<Okteta::ByteArrayTableLayout> mLayout;
    std::unique_ptr<Okteta::ByteArrayTableRanges> mTableRanges;

    std::unique_ptr<Okteta::PrintColumnStylist> mStylist;

protected:
    /** */
    std::unique_ptr<const Okteta::ValueCodec> mValueCodec;
    /** */
    Okteta::ValueCoding mValueCoding;
    /** */
    std::unique_ptr<const Okteta::CharCodec> mCharCodec;

protected: // parameters
    LayoutStyle mResizeStyle;
};

#endif
