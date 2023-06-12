/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYROWSCOLUMNHTMLRENDERER_HPP
#define KASTEN_BYTEARRAYROWSCOLUMNHTMLRENDERER_HPP

// lib
#include "abstractcolumnhtmlrenderer.hpp"
// Okteta gui
#include <Okteta/CoordRange>
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QChar>
// Std
#include <memory>

namespace Okteta {
class ValueCodec;
class CharCodec;
class AbstractByteArrayModel;
}

namespace Kasten {

// TODO: offset should be set in renderFirstLine, calculated using coordRange,
// in constructor instead take startOffset
class ByteArrayRowsColumnHtmlRenderer : public AbstractColumnHtmlRenderer
{
public:
    ByteArrayRowsColumnHtmlRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                    const Okteta::CoordRange& coordRange,
                                    int noOfBytesPerLine, int noOfGroupedBytes,
                                    int visibleCodings,
                                    Okteta::ValueCoding valueCoding,
                                    const QString& charCodecName, QChar substituteChar, QChar undefinedChar);
    ~ByteArrayRowsColumnHtmlRenderer() override;

public: // AbstractColumnHtmlRenderer API
    void renderFirstLine(QTextStream* stream, int lineIndex) const override;
    void renderNextLine(QTextStream* stream, bool isSubline) const override;
    int noOfSublinesNeeded() const override;

private:
    void renderLine(QTextStream* stream, bool isSubline) const;

private:
    const Okteta::AbstractByteArrayModel* const mByteArrayModel;

    const Okteta::CoordRange mCoordRange;

    const int mNoOfBytesPerLine;

    const int mVisibleCodings;
    std::unique_ptr<Okteta::ValueCodec> const mValueCodec;
    std::unique_ptr<Okteta::CharCodec> const mCharCodec;
    const QChar mSubstituteChar;
    const QChar mUndefinedChar;

    /** Line to print */
    mutable int mRenderLine;
    /** Data to print */
    mutable Okteta::Address mOffset;
};

}

#endif
