/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "offsetcolumnhtmlrenderer.hpp"

// Qt
#include <QString>
#include <QTextStream>

namespace Kasten {

OffsetColumnHtmlRenderer::OffsetColumnHtmlRenderer(int offsetFormat, int firstLineOffset, int delta, bool columnMode)
    : mFirstLineOffset(firstLineOffset)
    , mDelta(delta)
    , printFunction(Okteta::OffsetFormat::printFunction(offsetFormat))
    , mCodingWidth(Okteta::OffsetFormat::codingWidth(offsetFormat))
    , mColumnMode(columnMode)
    , mEncodedOffsetBuffer(new char[mCodingWidth + 1])
{
}

OffsetColumnHtmlRenderer::~OffsetColumnHtmlRenderer() = default;

void OffsetColumnHtmlRenderer::renderFirstLine(QTextStream* stream, int lineIndex) const
{
    mRenderLine = lineIndex;
    renderLine(stream, false);
}

void OffsetColumnHtmlRenderer::renderNextLine(QTextStream* stream, bool isSubline) const
{
    renderLine(stream, isSubline);
}

void OffsetColumnHtmlRenderer::renderLine(QTextStream* stream, bool isSubline) const
{
    if (mColumnMode) {
        *stream << "<th nowrap>";
    } else {
        if (!isSubline) {
            *stream << "<th nowrap rowspan=2 valign=top>";
        }
    }
    if (!isSubline) {
        // TODO: fix me (no more printFunction)
        printFunction(mEncodedOffsetBuffer.get(), mFirstLineOffset + mDelta * mRenderLine);
        *stream << "<tt>" << mEncodedOffsetBuffer.get() << "</tt>";

        ++mRenderLine;
    }
    if (mColumnMode || !isSubline) {
        *stream << "</th>";
    }
}

}
