/*
    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_HEXADECIMALBYTECODEC_HPP
#define OKTETA_HEXADECIMALBYTECODEC_HPP

// lib
#include <valuecodec.hpp>
// Qt
#include <QChar>
// Std
#include <array>

class QChar;

namespace Okteta {

class HexadecimalByteCodec : public ValueCodec
{
public:
    explicit HexadecimalByteCodec(bool lowerCaseDigits = false);
    HexadecimalByteCodec(const HexadecimalByteCodec&) = delete;
    HexadecimalByteCodec(HexadecimalByteCodec&&) = delete;
    HexadecimalByteCodec& operator=(const HexadecimalByteCodec&) = delete;
    HexadecimalByteCodec& operator=(HexadecimalByteCodec&&) = delete;

public:
    bool setLowerCaseDigits(bool lowerCaseDigits);
    [[nodiscard]]
    bool isLowerCaseDigits() const;

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

private:
    using DigitsTable = std::array<QChar, 16>;

    static const DigitsTable upperCaseDigits;
    static const DigitsTable lowerCaseDigits;

private:
    const QChar* mDigits;
};

}

#endif
