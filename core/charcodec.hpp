/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHARCODEC_HPP
#define OKTETA_CHARCODEC_HPP

// lib
#include "oktetacore_export.hpp"
#include "byte.hpp"
// Std
#include <memory>

class QStringList;
class QString;
class QChar;

namespace Okteta {

class Character;

class OKTETACORE_EXPORT CharCodec
{
public:
    virtual ~CharCodec();

public:
    /**
     * If @p name is empty, creates a system-local 8-bit char codec
     */
    static std::unique_ptr<CharCodec> createCodec(const QString& name);
    static std::unique_ptr<CharCodec> createCodecForLocale();

    [[nodiscard]]
    static QStringList codecNames();

public: // API to be implemented
    virtual Character decode(Byte byte) const = 0;
    virtual bool encode(Byte* byte, QChar _char) const = 0;
    [[nodiscard]]
    virtual bool canEncode(QChar _char) const = 0;
    [[nodiscard]]
    virtual QString name() const = 0;
};

inline CharCodec::~CharCodec() = default;

}

#endif
