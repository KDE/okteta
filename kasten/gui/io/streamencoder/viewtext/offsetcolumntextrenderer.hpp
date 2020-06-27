/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_OFFSETCOLUMNTEXTRENDERER_HPP
#define KASTEN_OFFSETCOLUMNTEXTRENDERER_HPP

// lib
#include "abstractcolumntextrenderer.hpp"
// Okteta gui
#include <Okteta/OffsetFormat>

namespace Kasten {

class OffsetColumnTextRenderer : public AbstractColumnTextRenderer
{
public:
    OffsetColumnTextRenderer(int offsetFormat, int firstLineOffset, int delta);
    ~OffsetColumnTextRenderer() override;

public: // AbstractColumnTextRenderer API
    void renderFirstLine(QTextStream* stream, int lineIndex) const override;
    void renderNextLine(QTextStream* stream, bool isSubline) const override;

private:
    void renderLine(QTextStream* stream, bool isSubline) const;

private:
    const int mFirstLineOffset;
    const int mDelta;
    const Okteta::OffsetFormat::print printFunction;
    const int mCodingWidth;

    /** the line we are in */
    mutable int mRenderLine;
    mutable char* mEncodedOffsetBuffer;
};

}

#endif
