/*
    SPDX-FileCopyrightText: 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_USASCIICHARCODEC_HPP
#define OKTETA_USASCIICHARCODEC_HPP

// lib
#include <charcodec.hpp>

namespace Okteta {

//
class USASCIICharCodec : public CharCodec
{
    struct ConstructorTag
    {
        ConstructorTag() = default;
        ConstructorTag(const ConstructorTag&) = default;
    };

public:
    explicit USASCIICharCodec(ConstructorTag);
    USASCIICharCodec(const USASCIICharCodec&) = delete;
    USASCIICharCodec(USASCIICharCodec&&) = delete;
    USASCIICharCodec& operator=(const USASCIICharCodec&) = delete;
    USASCIICharCodec& operator=(USASCIICharCodec&&) = delete;

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
    static std::unique_ptr<USASCIICharCodec> create();
    [[nodiscard]]
    static const QString& codecName();
};

inline USASCIICharCodec::USASCIICharCodec(USASCIICharCodec::ConstructorTag) {}

inline std::unique_ptr<USASCIICharCodec> USASCIICharCodec::create() { return std::make_unique<USASCIICharCodec>(ConstructorTag()); }

}

#endif
