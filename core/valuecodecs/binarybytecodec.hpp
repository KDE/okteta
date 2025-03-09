/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BINARYBYTECODEC_HPP
#define OKTETA_BINARYBYTECODEC_HPP

// lib
#include <valuecodec.hpp>

namespace Okteta {

class BinaryByteCodec : public ValueCodec
{
public:
    BinaryByteCodec() = default;
    BinaryByteCodec(const BinaryByteCodec&) = delete;
    BinaryByteCodec(BinaryByteCodec&&) = delete;
    BinaryByteCodec& operator=(const BinaryByteCodec&) = delete;
    BinaryByteCodec& operator=(BinaryByteCodec&&) = delete;

public: // ValueCodec API
    [[nodiscard]]
    unsigned int encodingWidth() const override;
    [[nodiscard]]
    Byte digitsFilledLimit() const override;

    void encode(QString* digits, unsigned int pos, Byte byte) const override;
    void encodeShort(QString* digits, unsigned int pos, Byte byte) const override;
    [[nodiscard]]
    bool appendDigit(Byte* byte, unsigned char digit) const override;
    void removeLastDigit(Byte* byte) const override;
    [[nodiscard]]
    bool isValidDigit(unsigned char digit) const override;
    [[nodiscard]]
    bool turnToValue(unsigned char* digit) const override;
};

}

#endif
