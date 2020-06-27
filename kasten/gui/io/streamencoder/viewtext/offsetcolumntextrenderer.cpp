/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "offsetcolumntextrenderer.hpp"

// Qt
#include <QString>
#include <QTextStream>

namespace Kasten {

OffsetColumnTextRenderer::OffsetColumnTextRenderer(int offsetFormat, int firstLineOffset, int delta)
    : mFirstLineOffset(firstLineOffset)
    , mDelta(delta)
    , printFunction(Okteta::OffsetFormat::printFunction(offsetFormat))
    , mCodingWidth(Okteta::OffsetFormat::codingWidth(offsetFormat))
{
    mEncodedOffsetBuffer = new char[mCodingWidth + 1];
}

OffsetColumnTextRenderer::~OffsetColumnTextRenderer()
{
    delete [] mEncodedOffsetBuffer;
}

void OffsetColumnTextRenderer::renderFirstLine(QTextStream* stream, int lineIndex) const
{
    mRenderLine = lineIndex;
    renderLine(stream, false);
}

void OffsetColumnTextRenderer::renderNextLine(QTextStream* stream, bool isSubline) const
{
    renderLine(stream, isSubline);
}

void OffsetColumnTextRenderer::renderLine(QTextStream* stream, bool isSubline) const
{
    if (isSubline) {
        *stream << whiteSpace(mCodingWidth);
    } else {
        // TODO: fix me (no more printFunction)
        printFunction(mEncodedOffsetBuffer, mFirstLineOffset + mDelta * mRenderLine);
        *stream << mEncodedOffsetBuffer;

        ++mRenderLine;
    }
}

}
