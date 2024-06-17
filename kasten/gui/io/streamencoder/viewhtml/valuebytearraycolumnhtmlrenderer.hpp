/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VALUEBYTEARRAYCOLUMNHTMLRENDERER_HPP
#define KASTEN_VALUEBYTEARRAYCOLUMNHTMLRENDERER_HPP

// lib
#include "abstractbytearraycolumnhtmlrenderer.hpp"
// Okteta core
#include <Okteta/OktetaCore>
// Std
#include <memory>

namespace Okteta {
class ValueCodec;
}

namespace Kasten {

class ValueByteArrayColumnHtmlRenderer : public AbstractByteArrayColumnHtmlRenderer
{
public:
    ValueByteArrayColumnHtmlRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                     const Okteta::CoordRange& coordRange,
                                     int noOfBytesPerLine, int noOfGroupedBytes,
                                     Okteta::ValueCoding valueCoding);

    ~ValueByteArrayColumnHtmlRenderer() override;

protected: // AbstractByteArrayColumnHtmlRenderer API
    void renderLine(QTextStream* stream, bool isSubline) const override;

private:
    const std::unique_ptr<const Okteta::ValueCodec> mValueCodec;
};

}

#endif
