/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHAR8STRINGPARSER_HPP
#define OKTETA_CHAR8STRINGPARSER_HPP

// Qt
#include <QRegularExpression>

namespace Okteta {

class CharCodec;

class Char8StringParser
{
public:
    enum CodeState {
        CodeInvalid,
        CodeIntermediate,
        CodeAcceptable,
    };

public:
    explicit Char8StringParser(const CharCodec* charCodec);

public:
    [[nodiscard]]
    CodeState evaluate(QChar* result, const QString& string) const;
    [[nodiscard]]
    const Okteta::CharCodec* charCodec() const;

private:
    const CharCodec* const m_charCodec;

    const QRegularExpression m_octalNumberMatcher;
    const QRegularExpression m_hexNumberMatcher;
};

inline const Okteta::CharCodec* Char8StringParser::charCodec() const { return m_charCodec; }

}

#endif
