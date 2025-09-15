/*
    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_FLOAT32_HPP
#define OKTETA_FLOAT32_HPP

// Qt
#include <QLocale>
#include <QMetaType>
#include <QString>

namespace Okteta {

struct Float32
{
public:
    Float32(float v);
    Float32(const Float32&) = default;
    Float32(Float32&&) = default;
    Float32();

    ~Float32() = default;

    Float32& operator=(const Float32&) = default;
    Float32& operator=(Float32&&) = default;

public:
    [[nodiscard]]
    QString toString() const;
    [[nodiscard]]
    QString toString(const QLocale& locale) const;

public:
    float value = 0;
};

inline Float32::Float32() = default;
inline Float32::Float32(float v) : value(v) {}

inline QString Float32::toString() const
{
    return QString::number(value, 'e', 8);
}

inline QString Float32::toString(const QLocale& locale) const
{
    return locale.toString(value, 'e', 16);
}

}

Q_DECLARE_METATYPE(Okteta::Float32)

#endif
