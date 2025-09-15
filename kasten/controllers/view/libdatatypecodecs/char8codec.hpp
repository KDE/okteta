/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHAR8CODEC_HPP
#define OKTETA_CHAR8CODEC_HPP

// lib
#include "abstracttypecodec.hpp"

namespace Okteta {
class CharCodec;

class Char8Codec : public AbstractTypeCodec
{
public:
    explicit Char8Codec(const CharCodec* charCodec);

    ~Char8Codec() override;

public: // AbstractTypeCodec API
    [[nodiscard]]
    QVariant value(const PODData& data, int* byteCount) const override;
    [[nodiscard]]
    QByteArray valueToBytes(const QVariant& value) const override;

    [[nodiscard]]
    bool areEqual(const QVariant& value, QVariant& otherValue) const override;

public:
    void setCharCodec(const CharCodec* charCodec);

private:
    const CharCodec* mCharCodec;
    QChar mUndefinedChar;
};

inline void Char8Codec::setCharCodec(const CharCodec* charCodec) { mCharCodec = charCodec; }

}

#endif
