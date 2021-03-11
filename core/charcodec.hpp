/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHARCODEC_HPP
#define OKTETA_CHARCODEC_HPP

// lib
#include "oktetacore_export.hpp"
#include "oktetacore.hpp"
#include "byte.hpp"

class QStringList;
class QChar;

namespace Okteta {

class Character;

class OKTETACORE_EXPORT CharCodec
{
public:
    virtual ~CharCodec();

public:
    /** */
    static CharCodec* createCodec(CharCoding charCoding);
    /** */
    static CharCodec* createCodec(const QString& name);

    static const QStringList& codecNames();

public: // API to be implemented
    virtual Character decode(Byte byte) const = 0;
    virtual bool encode(Byte* byte, const QChar& _char) const = 0;
    virtual bool canEncode(const QChar& _char) const = 0;
    virtual const QString& name() const = 0;
};

inline CharCodec::~CharCodec() = default;

}

#endif
