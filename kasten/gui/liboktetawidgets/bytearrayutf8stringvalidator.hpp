/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYUTF8STRINGVALIDATOR_HPP
#define OKTETA_BYTEARRAYUTF8STRINGVALIDATOR_HPP

// lib
#include "abstractbytearraystringvalidator.hpp"
#include "bytearrayutf8stringdecoder.hpp"
#include "bytearrayutf8stringencoder.hpp"

namespace Okteta {

class ByteArrayUtf8StringValidator : public AbstractByteArrayStringValidator
{
public:
    ByteArrayUtf8StringValidator();

    ~ByteArrayUtf8StringValidator() override;

public: // AbstractByteArrayStringValidator API
    [[nodiscard]]
    QValidator::State validate(QString& input, int& pos) const override;

    [[nodiscard]]
    QByteArray toByteArray(const QString& string) const override;
    [[nodiscard]]
    QString toString(const QByteArray& byteArray) const override;

private:
    ByteArrayUtf8StringDecoder m_decoder;
    ByteArrayUtf8StringEncoder m_encoder;
};

}

#endif
