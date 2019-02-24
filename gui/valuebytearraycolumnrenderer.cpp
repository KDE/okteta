/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2008,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
