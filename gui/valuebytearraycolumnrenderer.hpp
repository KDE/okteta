/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_VALUEBYTEARRAYCOLUMNRENDERER_HPP
#define OKTETA_VALUEBYTEARRAYCOLUMNRENDERER_HPP

// lib
#include "abstractbytearraycolumnrenderer.hpp"
// Okteta core
#include <Okteta/Byte>

namespace Okteta {

class ValueCodec;

class ValueByteArrayColumnRendererPrivate;

/** buffer column which displays the numerical values of the bytes
 *
 * @author Friedrich W. H. Kossebau
 */
class OKTETAGUI_EXPORT ValueByteArrayColumnRenderer : public AbstractByteArrayColumnRenderer
{
public:
    ValueByteArrayColumnRenderer(AbstractColumnStylist* stylist,
                                 AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges);
    ValueByteArrayColumnRenderer() = delete;

    ~ValueByteArrayColumnRenderer() override;

public:
    void renderEditedByte(QPainter* painter, Byte byte, const QString& editBuffer);

public: // modification access
    void setValueCodec(ValueCoding valueCoding, const ValueCodec* valueCodec);
    /** sets the spacing in the middle of a binary byte in the value column
     * @param binaryGapWidth spacing in the middle of a binary in pixels
     * returns true if there was a change
     */
    bool setBinaryGapWidth(PixelX binaryGapWidth);

public: // value access
    PixelX binaryGapWidth() const;

private:
    Q_DECLARE_PRIVATE(ValueByteArrayColumnRenderer)
};

}

#endif
