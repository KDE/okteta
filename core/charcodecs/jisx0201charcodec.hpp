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
public:
    JISX0201CharCodec(const JISX0201CharCodec&) = delete;
    JISX0201CharCodec& operator=(const JISX0201CharCodec&) = delete;

protected:
    JISX0201CharCodec();

public: // CharCodec API
    Character decode(Byte byte) const override;
    bool encode(Byte* byte, QChar _char) const override;
    bool canEncode(QChar _char) const override;
    QString name() const override;

public:
    static JISX0201CharCodec* create();
    static const QString& codecName();
};

inline JISX0201CharCodec::JISX0201CharCodec() = default;

inline JISX0201CharCodec* JISX0201CharCodec::create() { return new JISX0201CharCodec(); }

}

#endif
