/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYROWFRAMERENDERER_HPP
#define KASTEN_BYTEARRAYROWFRAMERENDERER_HPP

// lib
#include "abstractbytearrayframerenderer.hpp"

namespace Okteta {
class OffsetColumnRenderer;
class BorderColumnRenderer;
class ByteArrayRowColumnRenderer;
}

class ByteArrayRowFrameRenderer : public AbstractByteArrayFrameRenderer
{
public:
    ByteArrayRowFrameRenderer();
    ~ByteArrayRowFrameRenderer() override;

public:
    Okteta::PixelX byteSpacingWidth() const;
    int noOfGroupedBytes() const;
    Okteta::PixelX groupSpacingWidth() const;
    Okteta::PixelX binaryGapWidth() const;
    bool showsNonprinting() const;
    QChar substituteChar() const;
    QChar undefinedChar() const;

    bool offsetColumnVisible() const;
    Okteta::OffsetFormat::Format offsetCoding() const;

    int visibleByteArrayCodings() const;

public: // AbstractByteArrayFrameRenderer API
    void setByteArrayModel(Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset = 0, Okteta::Size length = -1) override;
    void setFont(const QFont& font) override;
    void setBufferSpacing(Okteta::PixelX byteSpacing, int noOfGroupedBytes, Okteta::PixelX groupSpacing) override;
    void setValueCoding(Okteta::ValueCoding valueCoding) override;
    void setByteSpacingWidth(Okteta::PixelX byteSpacingWidth) override;
    void setNoOfGroupedBytes(int noOfGroupedBytes) override;
    void setGroupSpacingWidth(Okteta::PixelX groupSpacingWidth) override;
    void setBinaryGapWidth(Okteta::PixelX binaryGapWidth) override;
    void setSubstituteChar(QChar substituteChar) override;
    void setUndefinedChar(QChar undefinedChar) override;
    void setShowsNonprinting(bool showsNonprinting) override;
    void setCharCoding(const QString& charCodingName) override;
    void showByteArrayColumns(int CCs) override;
    void showOffsetColumn(bool visible) override;
    void setOffsetCoding(Okteta::OffsetFormat::Format offsetCoding) override;

protected: // AbstractColumnFrameRenderer API
//     virtual void setNoOfLines( int newNoOfLines );

protected:
    void adjustToLayoutNoOfBytesPerLine() override;

    int fittingBytesPerLine() const override;

private:
    Okteta::OffsetColumnRenderer*        mOffsetColumnRenderer;
    Okteta::BorderColumnRenderer*        mFirstBorderColumnRenderer;
    Okteta::ByteArrayRowColumnRenderer*  mByteArrayColumnRenderer;
};

#endif
