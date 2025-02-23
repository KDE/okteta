/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_VALUECODEC_HPP
#define OKTETA_VALUECODEC_HPP

// lib
#include "oktetacore.hpp"
#include "oktetacore_export.hpp"
#include "byte.hpp"
// Std
#include <memory>

class QString;

namespace Okteta {

/** Class that is able to convert codings to and from
 * hexadecimal, decimal, octal, and binary.
 *
 * The buffer will be always filled up to CodingWidth, if not using shortCodingFunction.
 *
 * @author Friedrich W. H. Kossebau
 */

class OKTETACORE_EXPORT ValueCodec
{
public:
    [[nodiscard]]
    static std::unique_ptr<ValueCodec> createCodec(ValueCoding valueCoding);

public:
    virtual ~ValueCodec();

protected:
    ValueCodec();

public: // API to be implemented
    /**
     * @return number of digits the greatest byte gets decoded into
     */
    [[nodiscard]]
    virtual unsigned int encodingWidth() const = 0;
    /**
     * @return largest value which could get another digit appended and stay below 256.
     */
    [[nodiscard]]
    virtual Byte digitsFilledLimit() const = 0;

    /**
     * Encodes the byte using full coding width, prefixing with 0s if needed,
     * and writes the result to digits
     * @param digits string to write the digits into
     * @param pos offset in digits where to write the code to
     * @param byte data to encode
     */
    virtual void encode(QString* digits, unsigned int pos, Byte byte) const = 0;
    /**
     * Encodes the byte and writes the result to digits, no leading 0s.
     * @param digits string to write the digits into
     * @param pos offset in digits where to write the code to
     * @param byte data to encode
     */
    virtual void encodeShort(QString* digits, unsigned int pos, Byte byte) const = 0;

    /**
     * Tries to increase the byte value by appending a digit to the digits of
     * the current encoding.
     * @param digit digit to append to encoding of current value.
     * @param byte offset in digits where to write the code to
     * @return true if successful, false otherwise
     */
    [[nodiscard]]
    virtual bool appendDigit(Byte* byte, unsigned char digit) const = 0;
    /**
     * Tries to remove the last (least significant) digit from byte.
     * @param byte value to remove the last digit from
     * @return true if successful, false otherwise
     */
    virtual void removeLastDigit(Byte* byte) const = 0;
    /**
     * Checks if the given digit is used in the encoding.
     * @param digit value to check
     * @return true if digit is valid, false otherwise
     */
    [[nodiscard]]
    virtual bool isValidDigit(unsigned char digit) const = 0;
    /**
     * Turns the digit into a byte with the value of the digit.
     * @param digit digit to turn into the value
     * @return true if successful, false otherwise
     */
    [[nodiscard]]
    virtual bool turnToValue(unsigned char* digit) const = 0;

public:
    /**
     * Tries to decode the digits in the text into a byte.
     * @param byte pointer to the variable to store the result in
     * @param text string to turn into the value
     * @param pos offset in the text to start with decoding
     * @return used chars of the string for the decoding, beginning with pos
     */
    unsigned int decode(Byte* byte, const QString& digits, unsigned int pos = 0) const;
};

inline ValueCodec::ValueCodec() = default;

}

#endif
