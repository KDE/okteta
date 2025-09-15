/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_UTF8CODEC_HPP
#define OKTETA_UTF8CODEC_HPP

// lib
#include "abstracttypecodec.hpp"

class QTextCodec;

namespace Okteta {

class Utf8Codec : public AbstractTypeCodec
{
public:
    Utf8Codec();

    ~Utf8Codec() override;

public: // AbstractTypeCodec API
    [[nodiscard]]
    QVariant value(const PODData& data, int* byteCount) const override;
    [[nodiscard]]
    QByteArray valueToBytes(const QVariant& value) const override;

    [[nodiscard]]
    bool areEqual(const QVariant& value, QVariant& otherValue) const override;

private:
    // Qt owns all QTextCodec instances
    QTextCodec* const mUtf8Codec;
};

}

#endif
