/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYVALUESTRINGVALIDATOR_HPP
#define OKTETA_BYTEARRAYVALUESTRINGVALIDATOR_HPP

// lib
#include "abstractbytearraystringvalidator.hpp"
// Std
#include <memory>

namespace Okteta {
class ValueCodec;

class ByteArrayValueStringValidator : public AbstractByteArrayStringValidator
{
public:
    // matching Okteta::ValueCoding
    enum Coding
    {
        HexadecimalCoding = 0,
        DecimalCoding = 1,
        OctalCoding = 2,
        BinaryCoding = 3,
    };

public:
    explicit ByteArrayValueStringValidator(Coding valueCodingId = HexadecimalCoding);

    ~ByteArrayValueStringValidator() override;

public: // AbstractByteArrayStringValidator API
    QValidator::State validate(QString& input, int& pos) const override;

    QByteArray toByteArray(const QString& string) const override;
    QString toString(const QByteArray& byteArray) const override;

public:
    /// Sets one of the value codecs or the current char codec.
    void setCoding(Coding valueCodingId);

public:
    Coding coding() const;

private:
    // TODO: add Okteta::ValueCodec::coding() query method, use that instead of separate Coding type tracker
    std::unique_ptr<const ValueCodec> m_valueCodec;
    Coding m_codecId;
};

inline ByteArrayValueStringValidator::Coding ByteArrayValueStringValidator::coding() const { return m_codecId; }

}

#endif
