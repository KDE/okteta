/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCOLUMNFRAMERENDERER_HPP
#define KASTEN_BYTEARRAYCOLUMNFRAMERENDERER_HPP

// lib
#include "abstractbytearrayframerenderer.hpp"

namespace Okteta {
class OffsetColumnRenderer;
class BorderColumnRenderer;
class ValueByteArrayColumnRenderer;
class CharByteArrayColumnRenderer;
}

class ByteArrayColumnFrameRenderer : public AbstractByteArrayFrameRenderer
{
public:
    ByteArrayColumnFrameRenderer();
    ~ByteArrayColumnFrameRenderer() override;

public:
    [[nodiscard]]
    Okteta::PixelX byteSpacingWidth() const;
    [[nodiscard]]
    int noOfGroupedBytes() const;
    [[nodiscard]]
    Okteta::PixelX groupSpacingWidth() const;
    [[nodiscard]]
    Okteta::PixelX binaryGapWidth() const;
    [[nodiscard]]
    bool showsNonprinting() const;
    [[nodiscard]]
    QChar substituteChar() const;
    [[nodiscard]]
    QChar undefinedChar() const;

    [[nodiscard]]
    bool offsetColumnVisible() const;
    [[nodiscard]]
    Okteta::OffsetFormat::Format offsetCoding() const;

    [[nodiscard]]
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

    [[nodiscard]]
    int fittingBytesPerLine() const override;

private:
    Okteta::OffsetColumnRenderer*         mOffsetColumnRenderer;
    Okteta::BorderColumnRenderer*         mFirstBorderColumnRenderer;
    Okteta::ValueByteArrayColumnRenderer* mValueColumnRenderer;
    Okteta::BorderColumnRenderer*         mSecondBorderColumnRenderer;
    Okteta::CharByteArrayColumnRenderer*  mCharColumnRenderer;
};

#endif
