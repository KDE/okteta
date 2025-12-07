/*
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

template <class T> class QList;
class QString;
using QStringList = QList<QString>;
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
    [[nodiscard]]
    static std::unique_ptr<CharCodec> createCodec(const QString& name);
    [[nodiscard]]
    static std::unique_ptr<CharCodec> createCodecForLocale();

    [[nodiscard]]
    static QStringList codecNames();

public: // API to be implemented
    [[nodiscard]]
    virtual Character decode(Byte byte) const = 0;
    [[nodiscard]]
    virtual bool encode(Byte* byte, QChar _char) const = 0;
    [[nodiscard]]
    virtual bool canEncode(QChar _char) const = 0;
    [[nodiscard]]
    virtual QString name() const = 0;
};

inline CharCodec::~CharCodec() = default;

}

#endif
