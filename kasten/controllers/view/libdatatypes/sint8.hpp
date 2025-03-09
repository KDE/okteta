/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_SINT8_HPP
#define OKTETA_SINT8_HPP

// Qt
#include <QLocale>
#include <QMetaType>
#include <QString>

namespace Okteta {

struct SInt8
{
public:
    SInt8(qint8 v);
    SInt8(const SInt8&) = default;
    SInt8(SInt8&&) = default;
    SInt8();

    ~SInt8() = default;

    SInt8& operator=(const SInt8&) = default;
    SInt8& operator=(SInt8&&) = default;

public:
    [[nodiscard]]
    QString toString() const;
    [[nodiscard]]
    QString toString(const QLocale& locale) const;

public:
    qint8 value = 0;
};

inline SInt8::SInt8() = default;
inline SInt8::SInt8(qint8 v) : value(v) {}

inline QString SInt8::toString() const { return QString::number(value); }
inline QString SInt8::toString(const QLocale& locale) const { return locale.toString(value); }

}

Q_DECLARE_METATYPE(Okteta::SInt8)

#endif
