/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYROWSCOLUMNTEXTRENDERER_HPP
#define KASTEN_BYTEARRAYROWSCOLUMNTEXTRENDERER_HPP

// lib
#include "abstractcolumntextrenderer.hpp"
// Okteta gui
#include <Okteta/CoordRange>
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QChar>

namespace Okteta {
class ValueCodec;
class CharCodec;
class AbstractByteArrayModel;
}

namespace Kasten {

// TODO: offset should be set in renderFirstLine, calculated using coordRange,
// in constructor instead take startOffset
class ByteArrayRowsColumnTextRenderer : public AbstractColumnTextRenderer
{
private:
    static constexpr int DefaultTRByteSpacingWidth = 1;
    static constexpr int TRGroupSpacingWidth = 2;

public:
    ByteArrayRowsColumnTextRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                    const Okteta::CoordRange& coordRange,
                                    int noOfBytesPerLine, int byteSpacingWidth, int noOfGroupedBytes,
                                    int visibleCodings,
                                    Okteta::ValueCoding valueCoding,
                                    const QString& charCodecName, QChar substituteChar, QChar undefinedChar);
    ~ByteArrayRowsColumnTextRenderer() override;

public: // AbstractColumnTextRenderer API
    void renderFirstLine(QTextStream* stream, int lineIndex) const override;
    void renderNextLine(QTextStream* stream, bool isSubline) const override;
    int noOfSublinesNeeded() const override;

private:
    void renderLine(QTextStream* stream, bool isSubline) const;

private:
    void setWidths(int byteWidth, int byteSpacingWidth, int noOfGroupedBytes);

private:
    const Okteta::AbstractByteArrayModel* mByteArrayModel;

    const Okteta::CoordRange mCoordRange;

    const int mNoOfBytesPerLine;

    const int mVisibleCodings;
    const Okteta::ValueCodec* mValueCodec;
    const Okteta::CharCodec* mCharCodec;
    const QChar mSubstituteChar;
    const QChar mUndefinedChar;

    /** Line to print */
    mutable int mRenderLine;
    /** Data to print */
    mutable Okteta::Address mOffset;

    /** buffered value of how many chars a line needs */
    int mNoOfCharsPerLine = 0;
    // positions where to paint the
    int* const mLinePositions;
};

}

#endif
