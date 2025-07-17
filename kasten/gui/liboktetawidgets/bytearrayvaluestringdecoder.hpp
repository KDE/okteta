/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYVALUESTRINGDECODER_HPP
#define OKTETA_BYTEARRAYVALUESTRINGDECODER_HPP

class QByteArray;
class QString;

namespace Okteta {

class ValueCodec;

// TODO: make ByteArrayValueStringValidator use this
// TODO: turn into some public Okteta Core decoder class
class ByteArrayValueStringDecoder
{
public:
    enum CodeState {
        CodeInvalid,
        CodeIntermediate,
        CodeAcceptable,
    };

public:
    explicit ByteArrayValueStringDecoder(const ValueCodec* valueCodec);
    ByteArrayValueStringDecoder(const ByteArrayValueStringDecoder&) = delete;

    ~ByteArrayValueStringDecoder();

    ByteArrayValueStringDecoder& operator=(const ByteArrayValueStringDecoder&) = delete;

public:
    // TODO: add flags to control separator, prefixes/postfixes and leading/trailing spaces
    [[nodiscard]]
    CodeState decode(QByteArray* result, const QString& string) const;

private:
    const ValueCodec* const m_valueCodec;
};

}

#endif
