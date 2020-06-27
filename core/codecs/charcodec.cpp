/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charcodec.hpp"

// lib
#include "textcharcodec.hpp"
#include "ebcdic1047charcodec.hpp"
#include "jisx0201charcodec.hpp"
#include "usasciicharcodec.hpp"
// Qt
#include <QStringList>

namespace Okteta {

const QStringList& CharCodec::codecNames()
{
    static QStringList codecNames;
    // first call?
    if (codecNames.isEmpty()) {
        codecNames = TextCharCodec::codecNames();
        codecNames.reserve(codecNames.size() + 3);
        codecNames.append(USASCIICharCodec::codecName());
        codecNames.append(EBCDIC1047CharCodec::codecName());
        codecNames.append(JISX0201CharCodec::codecName());
    }

    return codecNames;
}

CharCodec* CharCodec::createCodec(const QString& name)
{
    CharCodec* result;

    if (TextCharCodec::codecNames().indexOf(name) != -1) {
        result = TextCharCodec::createCodec(name);
    } else if (USASCIICharCodec::codecName() == name) {
        result = USASCIICharCodec::create();
    } else if (EBCDIC1047CharCodec::codecName() == name) {
        result = EBCDIC1047CharCodec::create();
    } else if (JISX0201CharCodec::codecName() == name) {
        result = JISX0201CharCodec::create();
    } else {
        result = nullptr;
    }

    // ensure at least a codec
    if (!result) {
        result = TextCharCodec::createLocalCodec();
    }

    return result;
}

CharCodec* CharCodec::createCodec(CharCoding charCoding)
{
    CharCodec* result;

    if (charCoding == EBCDIC1047Encoding) {
        result = EBCDIC1047CharCodec::create();
    } else if (charCoding == ISO8859_1Encoding) {
        result = TextCharCodec::createCodec(QStringLiteral("ISO-8859-1"));
    }
    // LocalEncoding
    else {
        result = nullptr;
    }

    // ensure at least a codec
    if (!result) {
        result = TextCharCodec::createLocalCodec();
    }

    return result;
}

}
