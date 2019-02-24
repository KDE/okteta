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

#ifndef OKTETA_VALUEBYTEARRAYCOLUMNRENDERER_P_HPP
#define OKTETA_VALUEBYTEARRAYCOLUMNRENDERER_P_HPP

// lib
#include "valuebytearraycolumnrenderer.hpp"
#include "abstractbytearraycolumnrenderer_p.hpp"
#include "oktetagui.hpp"

namespace Okteta {

class ValueByteArrayColumnRendererPrivate : public AbstractByteArrayColumnRendererPrivate
{
public:
    ValueByteArrayColumnRendererPrivate(ValueByteArrayColumnRenderer* q,
                                        AbstractColumnStylist* stylist,
                                        AbstractByteArrayModel* byteArrayModel,
                                        ByteArrayTableLayout* layout,
                                        ByteArrayTableRanges* ranges);

    ~ValueByteArrayColumnRendererPrivate() override;

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

protected: // AbstractByteArrayColumnRendererPrivate API
    void renderByteText(QPainter* painter, Byte byte, Character byteChar, const QColor& color) const override;
    void recalcByteWidth() override;

protected:
    void renderCode(QPainter* painter, const QString& code, const QColor& color) const;

private: // settings
    /** */
    ValueCoding mValueCoding;
    /** */
    const ValueCodec* mValueCodec = nullptr;
    /** */
    PixelX mBinaryGapWidth;

private: // buffered data
    /** buffer to hold the formatted valueCoding */
    mutable QString mDecodedByteText;
    /** calculated: Offset in pixels of the second half of the binary */
    PixelX mBinaryHalfOffset;

private:
    Q_DECLARE_PUBLIC(ValueByteArrayColumnRenderer)
};


inline ValueByteArrayColumnRendererPrivate::ValueByteArrayColumnRendererPrivate(ValueByteArrayColumnRenderer* q,
                                                                                AbstractColumnStylist* stylist,
                                                                                AbstractByteArrayModel* byteArrayModel,
                                                                                ByteArrayTableLayout* layout,
                                                                                ByteArrayTableRanges* ranges)
    : AbstractByteArrayColumnRendererPrivate(q, stylist, byteArrayModel, layout, ranges)
    , mBinaryGapWidth(DefaultBinaryGapWidth)
{
}

inline ValueByteArrayColumnRendererPrivate::~ValueByteArrayColumnRendererPrivate() = default;

inline PixelX ValueByteArrayColumnRendererPrivate::binaryGapWidth() const { return mBinaryGapWidth; }

}

#endif
