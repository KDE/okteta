/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "valuebytearraycolumnhtmlrenderer.hpp"

// Okteta core
#include <Okteta/ValueCodec>
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QTextStream>

namespace Kasten {

ValueByteArrayColumnHtmlRenderer::ValueByteArrayColumnHtmlRenderer(
    const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
    const Okteta::CoordRange& coordRange,
    int noOfBytesPerLine, int noOfGroupedBytes,
    Okteta::ValueCoding valueCoding)
    : AbstractByteArrayColumnHtmlRenderer(byteArrayModel, offset, coordRange,
        noOfBytesPerLine)
    , mValueCodec(Okteta::ValueCodec::createCodec(valueCoding))
{
    Q_UNUSED(noOfGroupedBytes)
}

ValueByteArrayColumnHtmlRenderer::~ValueByteArrayColumnHtmlRenderer() = default;

void ValueByteArrayColumnHtmlRenderer::renderLine(QTextStream* stream, bool isSubline) const
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
    const bool needsFilledEmptyCells =
        (mCoordRange.lines() == 1) ? ((0 < p) || (pEnd < mNoOfBytesPerLine)) :
        (mCoordRange.lines() == 2) ? (mCoordRange.end().pos() + 1 < mCoordRange.start().pos() ) :
        /* eefault */                false;
    const unsigned int codingWidth = needsFilledEmptyCells ? mValueCodec->encodingWidth() : 0;

    *stream << emptyCells(p, codingWidth);

    QString E;
    E.resize(mValueCodec->encodingWidth());
    // draw individual chars
    for (; p < pEnd; ++p, ++mOffset) {
        mValueCodec->encode(&E, 0, mByteArrayModel->byte(mOffset));
        *stream << "<td><tt>" << E.toHtmlEscaped() << "</tt></td>";
    }

    *stream << emptyCells(mNoOfBytesPerLine - p, codingWidth);

    ++mRenderLine;
}

}
