/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearraycolumnhtmlrenderer.hpp"

namespace Kasten {

AbstractByteArrayColumnHtmlRenderer::AbstractByteArrayColumnHtmlRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset, const Okteta::CoordRange& coordRange,
                                                                         int noOfBytesPerLine)
    : mByteArrayModel(byteArrayModel)
    , mCoordRange(coordRange)
    , mNoOfBytesPerLine(noOfBytesPerLine)
    , mOffset(offset)
{
}

AbstractByteArrayColumnHtmlRenderer::~AbstractByteArrayColumnHtmlRenderer() = default;

void AbstractByteArrayColumnHtmlRenderer::renderFirstLine(QTextStream* stream, int lineIndex) const
{
    mRenderLine = lineIndex;
    renderLine(stream, false);
}

void AbstractByteArrayColumnHtmlRenderer::renderNextLine(QTextStream* stream, bool isSubline) const
{
    renderLine(stream, isSubline);
}

}
