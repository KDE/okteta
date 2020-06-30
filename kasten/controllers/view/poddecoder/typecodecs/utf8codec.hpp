/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UTF8CODEC_HPP
#define KASTEN_UTF8CODEC_HPP

// tool
#include "../abstracttypecodec.hpp"

class QTextCodec;

namespace Okteta {

class Utf8Codec : public AbstractTypeCodec
{
public:
    Utf8Codec();

    ~Utf8Codec() override;

public: // AbstractTypeCodec API
    QVariant value(const PODData& data, int* byteCount) const override;
    QByteArray valueToBytes(const QVariant& value) const override;

    bool areEqual(const QVariant& value, QVariant& otherValue) const override;

private:
    QTextCodec* mUtf8Codec;
};

}

#endif
