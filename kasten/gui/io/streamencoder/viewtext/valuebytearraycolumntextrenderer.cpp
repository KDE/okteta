/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "valuebytearraycolumntextrenderer.hpp"

// Okteta core
#include <Okteta/ValueCodec>
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QTextStream>

namespace Kasten {

ValueByteArrayColumnTextRenderer::ValueByteArrayColumnTextRenderer(
    const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
    const Okteta::CoordRange& coordRange,
    int noOfBytesPerLine, int byteSpacingWidth, int noOfGroupedBytes,
    Okteta::ValueCoding valueCoding)
    : AbstractByteArrayColumnTextRenderer(byteArrayModel, offset, coordRange,
        noOfBytesPerLine)
    , mValueCodec(Okteta::ValueCodec::createCodec(valueCoding))
{
    setWidths(mValueCodec->encodingWidth(), byteSpacingWidth, noOfGroupedBytes);
}

ValueByteArrayColumnTextRenderer::~ValueByteArrayColumnTextRenderer()
{
    delete mValueCodec;
}

void ValueByteArrayColumnTextRenderer::renderLine(QTextStream* stream, bool isSubline) const
{
    Q_UNUSED(isSubline)

    int p = 0;
    int pEnd = mNoOfBytesPerLine;
    // correct boundaries
    if (mRenderLine == mCoordRange.start().line()) {
        p = mCoordRange.start().pos();
    }
    if (mRenderLine == mCoordRange.end().line()) {
        pEnd = mCoordRange.end().pos() + 1;
    }

    QString E;
    E.resize(mValueCodec->encodingWidth());
    // draw individual chars
    uint e = 0;
    for (; p < pEnd; ++p, ++mOffset) {
        // get next position
        const uint t = mLinePositions[p];
        // clear spacing
        *stream << whiteSpace(t - e);
        mValueCodec->encode(&E, 0, mByteArrayModel->byte(mOffset));
        *stream << E;
        e = t + mValueCodec->encodingWidth();
    }

    *stream << whiteSpace(mNoOfCharsPerLine - e);
    ++mRenderLine;
}

}
