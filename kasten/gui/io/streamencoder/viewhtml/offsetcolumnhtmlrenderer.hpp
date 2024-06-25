/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_OFFSETCOLUMNHTMLRENDERER_HPP
#define KASTEN_OFFSETCOLUMNHTMLRENDERER_HPP

// lib
#include "abstractcolumnhtmlrenderer.hpp"
// Okteta gui
#include <Okteta/OffsetFormat>
// Std
#include <memory>

namespace Kasten {

class OffsetColumnHtmlRenderer : public AbstractColumnHtmlRenderer
{
public:
    OffsetColumnHtmlRenderer(int offsetFormat, int firstLineOffset, int delta, bool columnMode);
    ~OffsetColumnHtmlRenderer() override;

public: // AbstractColumnHtmlRenderer API
    void renderFirstLine(QTextStream* stream, int lineIndex) const override;
    void renderNextLine(QTextStream* stream, bool isSubline) const override;

private:
    void renderLine(QTextStream* stream, bool isSubline) const;

private:
    const int mFirstLineOffset;
    const int mDelta;
    const Okteta::OffsetFormat::print printFunction;
    const int mCodingWidth;
    const bool mColumnMode;

    /** the line we are in */
    mutable int mRenderLine;
    mutable std::unique_ptr<char[]> mEncodedOffsetBuffer;
};

}

#endif
