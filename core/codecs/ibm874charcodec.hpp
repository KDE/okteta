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
protected:
    IBM874CharCodec();

public: // CharCodec API
    Character decode(Byte byte) const override;
    bool encode(Byte* byte, const QChar& _char) const override;
    bool canEncode(const QChar& _char) const override;
    const QString& name() const override;

public:
    static IBM874CharCodec* create();
    static const QString& codecName();
};

inline IBM874CharCodec::IBM874CharCodec() = default;

inline IBM874CharCodec* IBM874CharCodec::create() { return new IBM874CharCodec(); }

}

#endif
