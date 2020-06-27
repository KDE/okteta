/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearraycolumntextrenderer.hpp"

// Qt
#include <QTextStream>

namespace Kasten {

AbstractByteArrayColumnTextRenderer::AbstractByteArrayColumnTextRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset, const Okteta::CoordRange& coordRange,
                                                                         int noOfBytesPerLine)
    : mByteArrayModel(byteArrayModel)
    , mCoordRange(coordRange)
    , mNoOfBytesPerLine(noOfBytesPerLine)
    , mOffset(offset)
    , mLinePositions(new int[mNoOfBytesPerLine])
{
}

AbstractByteArrayColumnTextRenderer::~AbstractByteArrayColumnTextRenderer()
{
    delete [] mLinePositions;
}

void AbstractByteArrayColumnTextRenderer::setWidths(int byteWidth, int byteSpacingWidth, int noOfGroupedBytes)
{
    // TODO: remove this hack and make it more general
    if (byteSpacingWidth > 0) {
        byteSpacingWidth = DefaultTRByteSpacingWidth;
    }

    int spacingTrigger = noOfGroupedBytes - 1;
    if (spacingTrigger < 0) {
        spacingTrigger = mNoOfBytesPerLine; // ensures to never trigger the group spacing

    }
    int N = 0;
    int p = 0;
    int gs = 0;
    int* P = mLinePositions;
    for (; P < &mLinePositions[mNoOfBytesPerLine]; ++P, ++p, ++gs) {
        *P = N;
        N += byteWidth;

        // is there a space behind the actual byte (if it is not the last)?
        if (gs == spacingTrigger) {
            N += TRGroupSpacingWidth;
            gs = -1;
        } else {
            N += byteSpacingWidth;
        }
    }

    N -= (gs == 0) ? TRGroupSpacingWidth : byteSpacingWidth;

    mNoOfCharsPerLine = N;
}

void AbstractByteArrayColumnTextRenderer::renderFirstLine(QTextStream* stream, int lineIndex) const
{
    mRenderLine = lineIndex;
    renderLine(stream, false);
}

void AbstractByteArrayColumnTextRenderer::renderNextLine(QTextStream* stream, bool isSubline) const
{
    renderLine(stream, isSubline);
}

}
