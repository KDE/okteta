/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_UTF16_HPP
#define OKTETA_UTF16_HPP

// Qt
#include <QMetaType>
#include <QString>

namespace Okteta {

struct Utf16
{
public:
    Utf16(QChar v);
    Utf16(const Utf16&) = default;
    Utf16(Utf16&&) = default;
    Utf16();

    ~Utf16() = default;

    Utf16& operator=(const Utf16&) = default;
    Utf16& operator=(Utf16&&) = default;

public:
    [[nodiscard]]
    QString toString() const;

public:
    QChar value;
};

inline Utf16::Utf16() = default;
inline Utf16::Utf16(QChar v) : value(v) {}

inline QString Utf16::toString() const { return QString(value); }

}

Q_DECLARE_METATYPE(Okteta::Utf16)

#endif
