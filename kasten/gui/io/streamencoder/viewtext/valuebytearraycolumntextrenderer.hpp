/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VALUEBYTEARRAYCOLUMNTEXTRENDERER_HPP
#define KASTEN_VALUEBYTEARRAYCOLUMNTEXTRENDERER_HPP

// lib
#include "abstractbytearraycolumntextrenderer.hpp"
// Okteta core
#include <Okteta/OktetaCore>

namespace Okteta {
class ValueCodec;
}

namespace Kasten {

class ValueByteArrayColumnTextRenderer : public AbstractByteArrayColumnTextRenderer
{
public:
    ValueByteArrayColumnTextRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                     const Okteta::CoordRange& coordRange,
                                     int noOfBytesPerLine, int byteSpacingWidth, int noOfGroupedBytes,
                                     Okteta::ValueCoding valueCoding);

    ~ValueByteArrayColumnTextRenderer() override;

protected: // AbstractByteArrayColumnTextRenderer API
    void renderLine(QTextStream* stream, bool isSubline) const override;

private:
    const Okteta::ValueCodec* mValueCodec;
};

}

#endif
