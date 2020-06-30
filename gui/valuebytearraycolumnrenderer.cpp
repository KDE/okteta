/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "valuebytearraycolumnrenderer.hpp"
#include "valuebytearraycolumnrenderer_p.hpp"

namespace Okteta {

ValueByteArrayColumnRenderer::ValueByteArrayColumnRenderer(AbstractColumnStylist* stylist,
                                                           AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges)
    : AbstractByteArrayColumnRenderer(new ValueByteArrayColumnRendererPrivate(this, stylist, byteArrayModel, layout, ranges))
{
}

ValueByteArrayColumnRenderer::~ValueByteArrayColumnRenderer() = default;

PixelX ValueByteArrayColumnRenderer::binaryGapWidth() const
{
    Q_D(const ValueByteArrayColumnRenderer);

    return d->binaryGapWidth();
}

void ValueByteArrayColumnRenderer::setValueCodec(ValueCoding valueCoding, const ValueCodec* valueCodec)
{
    Q_D(ValueByteArrayColumnRenderer);

    d->setValueCodec(valueCoding, valueCodec);
}

bool ValueByteArrayColumnRenderer::setBinaryGapWidth(PixelX binaryGapWidth)
{
    Q_D(ValueByteArrayColumnRenderer);

    return d->setBinaryGapWidth(binaryGapWidth);
}

// perhaps sometimes there will be a grammar
void ValueByteArrayColumnRenderer::renderEditedByte(QPainter* painter, Byte byte, const QString& editBuffer)
{
    Q_D(ValueByteArrayColumnRenderer);

    d->renderEditedByte(painter, byte, editBuffer);
}

}
