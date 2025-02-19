/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_IBM874CHARCODEC_HPP
#define OKTETA_IBM874CHARCODEC_HPP

// lib
#include <charcodec.hpp>

namespace Okteta {

// Qt using ICU maps this onto TIS-620, which is not 100 % correct
class IBM874CharCodec : public CharCodec
{
    struct ConstructorTag
    {
        ConstructorTag() = default;
        ConstructorTag(const ConstructorTag&) = default;
    };

public:
    explicit IBM874CharCodec(ConstructorTag);
    IBM874CharCodec(const IBM874CharCodec&) = delete;
    IBM874CharCodec& operator=(const IBM874CharCodec&) = delete;

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
    static std::unique_ptr<IBM874CharCodec> create();
    [[nodiscard]]
    static const QString& codecName();
};

inline IBM874CharCodec::IBM874CharCodec(IBM874CharCodec::ConstructorTag) {}

inline std::unique_ptr<IBM874CharCodec> IBM874CharCodec::create() { return std::make_unique<IBM874CharCodec>(ConstructorTag()); }

}

#endif
