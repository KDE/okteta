/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYCHAR8STRINGVALIDATOR_HPP
#define OKTETA_BYTEARRAYCHAR8STRINGVALIDATOR_HPP

// lib
#include "abstractbytearraystringvalidator.hpp"
#include "bytearraychar8stringdecoder.hpp"
#include "bytearraychar8stringencoder.hpp"

namespace Okteta {

class ByteArrayChar8StringValidator : public AbstractByteArrayStringValidator
{
public:
    ByteArrayChar8StringValidator();

    ~ByteArrayChar8StringValidator() override;

public: // AbstractByteArrayStringValidator API
    [[nodiscard]]
    QValidator::State validate(QString& input, int& pos) const override;

    [[nodiscard]]
    QByteArray toByteArray(const QString& string) const override;
    [[nodiscard]]
    QString toString(const QByteArray& byteArray) const override;

public:
    void setCharCodec(const QString& charCodecName);

private:
    ByteArrayChar8StringDecoder m_decoder;
    ByteArrayChar8StringEncoder m_encoder;
};

}

#endif
