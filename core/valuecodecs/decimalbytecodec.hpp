/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_DECIMALBYTECODEC_HPP
#define OKTETA_DECIMALBYTECODEC_HPP

// lib
#include <valuecodec.hpp>

namespace Okteta {

class DecimalByteCodec : public ValueCodec
{
public: // ValueCodec API
    unsigned int encodingWidth() const override;
    Byte digitsFilledLimit() const override;

    void encode(QString* digits, unsigned int pos, Byte byte) const override;
    void encodeShort(QString* digits, unsigned int pos, Byte byte) const override;
    bool appendDigit(Byte* byte, unsigned char digit) const override;
    void removeLastDigit(Byte* byte) const override;
    bool isValidDigit(unsigned char digit) const override;
    bool turnToValue(unsigned char* digit) const override;
};

}

#endif
