/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYCOLUMNHTMLRENDERER_HPP
#define KASTEN_ABSTRACTBYTEARRAYCOLUMNHTMLRENDERER_HPP

// lib
#include "abstractcolumnhtmlrenderer.hpp"
// Okteta gui
#include <Okteta/CoordRange>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

// TODO: offset should be set in renderFirstLine, calculated using coordRange,
// in constructor instead take startOffset
class AbstractByteArrayColumnHtmlRenderer : public AbstractColumnHtmlRenderer
{
public:
    AbstractByteArrayColumnHtmlRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                        const Okteta::CoordRange& coordRange,
                                        int noOfBytesPerLine);
    ~AbstractByteArrayColumnHtmlRenderer() override;

public: // AbstractColumnHtmlRenderer API
    void renderFirstLine(QTextStream* stream, int lineIndex) const override;
    void renderNextLine(QTextStream* stream, bool isSubline) const override;

protected: // API to be reimplemented by subclasses
    virtual void renderLine(QTextStream* stream, bool isSubline) const = 0;

protected:
    const Okteta::AbstractByteArrayModel* const mByteArrayModel;

    const Okteta::CoordRange mCoordRange;

    const int mNoOfBytesPerLine;

    /** Line to print */
    mutable int mRenderLine;
    /** Data to print */
    mutable Okteta::Address mOffset;
};

}

#endif
