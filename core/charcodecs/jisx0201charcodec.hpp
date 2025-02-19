/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2015 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_JISX0201CHARCODEC_HPP
#define OKTETA_JISX0201CHARCODEC_HPP

// lib
#include <charcodec.hpp>

namespace Okteta {

//
class JISX0201CharCodec : public CharCodec
{
    struct ConstructorTag
    {
        ConstructorTag() = default;
        ConstructorTag(const ConstructorTag&) = default;
    };

public:
    explicit JISX0201CharCodec(ConstructorTag);
    JISX0201CharCodec(const JISX0201CharCodec&) = delete;
    JISX0201CharCodec& operator=(const JISX0201CharCodec&) = delete;

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
    static std::unique_ptr<JISX0201CharCodec> create();
    [[nodiscard]]
    static const QString& codecName();
};

inline JISX0201CharCodec::JISX0201CharCodec(JISX0201CharCodec::ConstructorTag) {}

inline std::unique_ptr<JISX0201CharCodec> JISX0201CharCodec::create() { return std::make_unique<JISX0201CharCodec>(ConstructorTag()); }

}

#endif
