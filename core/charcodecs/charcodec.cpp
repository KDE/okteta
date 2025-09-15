/*
    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charcodec.hpp"

// lib
#include "textcharcodec.hpp"
#include "ebcdic1047charcodec.hpp"
#include "ibm874charcodec.hpp"
#include "jisx0201charcodec.hpp"
#include "usasciicharcodec.hpp"
// Qt
#include <QStringList>

namespace Okteta {

QStringList CharCodec::codecNames()
{
    static QStringList codecNames;
    // first call?
    if (codecNames.isEmpty()) {
        codecNames = TextCharCodec::codecNames();
        codecNames.reserve(codecNames.size() + 4);
        codecNames.insert(codecNames.size()-2, IBM874CharCodec::codecName()); // insert at place of TextCharCodec one
        codecNames.append(USASCIICharCodec::codecName());
        codecNames.append(EBCDIC1047CharCodec::codecName());
        codecNames.append(JISX0201CharCodec::codecName());
    }

    return codecNames;
}

std::unique_ptr<CharCodec> CharCodec::createCodec(const QString& name)
{
    std::unique_ptr<CharCodec> result;

    if (TextCharCodec::codecNames().indexOf(name) != -1) {
        result = TextCharCodec::createCodec(name);
    } else if (USASCIICharCodec::codecName() == name) {
        result = USASCIICharCodec::create();
    } else if (EBCDIC1047CharCodec::codecName() == name) {
        result = EBCDIC1047CharCodec::create();
    } else if (IBM874CharCodec::codecName() == name) {
        result = IBM874CharCodec::create();
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

std::unique_ptr<CharCodec> CharCodec::createCodecForLocale()
{
    return TextCharCodec::createLocalCodec();
}

}
