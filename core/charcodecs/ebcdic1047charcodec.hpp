/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_EBCDIC1047CHARCODEC_HPP
#define OKTETA_EBCDIC1047CHARCODEC_HPP

// lib
#include <charcodec.hpp>

namespace Okteta {

//
class EBCDIC1047CharCodec : public CharCodec
{
    struct ConstructorTag
    {
        ConstructorTag() {}
        ConstructorTag(const ConstructorTag&) = default;
    };

public:
    explicit EBCDIC1047CharCodec(ConstructorTag);
    EBCDIC1047CharCodec(const EBCDIC1047CharCodec&) = delete;
    EBCDIC1047CharCodec& operator=(const EBCDIC1047CharCodec&) = delete;

public: // CharCodec API
    [[nodiscard]]
    Character decode(Byte byte) const override;
    [[nodiscard]]
    bool encode(Byte* byte, QChar _char) const override;
    [[nodiscard]]
    bool canEncode(QChar _char) const override;
    [[nodiscard]]
    QString name() const override;

public:
    [[nodiscard]]
    static std::unique_ptr<EBCDIC1047CharCodec> create();
    [[nodiscard]]
    static const QString& codecName();
};

inline EBCDIC1047CharCodec::EBCDIC1047CharCodec(EBCDIC1047CharCodec::ConstructorTag) {}

inline std::unique_ptr<EBCDIC1047CharCodec> EBCDIC1047CharCodec::create() { return std::make_unique<EBCDIC1047CharCodec>(ConstructorTag()); }

}

#endif
