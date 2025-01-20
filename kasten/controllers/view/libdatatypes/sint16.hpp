/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_SINT16_HPP
#define OKTETA_SINT16_HPP

// Qt
#include <QLocale>
#include <QMetaType>
#include <QString>

namespace Okteta {

struct SInt16
{
public:
    SInt16(qint16 v);
    SInt16(const SInt16&) = default;
    SInt16();

    ~SInt16() = default;

    SInt16& operator=(const SInt16&) = default;

public:
    [[nodiscard]]
    QString toString() const;
    [[nodiscard]]
    QString toString(const QLocale& locale) const;

public:
    qint16 value = 0;
};

inline SInt16::SInt16() = default;
inline SInt16::SInt16(qint16 v) : value(v) {}

inline QString SInt16::toString() const { return QString::number(value); }
inline QString SInt16::toString(const QLocale& locale) const { return locale.toString(value); }

}

Q_DECLARE_METATYPE(Okteta::SInt16)

#endif
