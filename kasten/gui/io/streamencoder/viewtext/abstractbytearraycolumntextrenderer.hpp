/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYCOLUMNTEXTRENDERER_HPP
#define KASTEN_ABSTRACTBYTEARRAYCOLUMNTEXTRENDERER_HPP

// lib
#include "abstractcolumntextrenderer.hpp"
// Okteta gui
#include <Okteta/CoordRange>

namespace Okteta {
class AbstractByteArrayModel;
}

namespace Kasten {

// TODO: offset should be set in renderFirstLine, calculated using coordRange,
// in constructor instead take startOffset
class AbstractByteArrayColumnTextRenderer : public AbstractColumnTextRenderer
{
private:
    static constexpr int DefaultTRByteSpacingWidth = 1;
    static constexpr int TRGroupSpacingWidth = 2;

public:
    AbstractByteArrayColumnTextRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                        const Okteta::CoordRange& coordRange,
                                        int noOfBytesPerLine);
    ~AbstractByteArrayColumnTextRenderer() override;

public: // AbstractColumnTextRenderer API
    void renderFirstLine(QTextStream* stream, int lineIndex) const override;
    void renderNextLine(QTextStream* stream, bool isSubline) const override;

protected: // API to be reimplemented by subclasses
    virtual void renderLine(QTextStream* stream, bool isSubline) const = 0;

protected:
    void setWidths(int byteWidth, int byteSpacingWidth, int noOfGroupedBytes);

protected:
    const Okteta::AbstractByteArrayModel* mByteArrayModel;

    const Okteta::CoordRange mCoordRange;

    const int mNoOfBytesPerLine;

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
