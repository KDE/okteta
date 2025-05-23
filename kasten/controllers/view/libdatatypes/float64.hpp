/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_FLOAT64_HPP
#define OKTETA_FLOAT64_HPP

// Qt
#include <QLocale>
#include <QMetaType>
#include <QString>

namespace Okteta {

struct Float64
{
public:
    Float64(double v);
    Float64(const Float64&) = default;
    Float64(Float64&&) = default;
    Float64();

    ~Float64() = default;

    Float64& operator=(const Float64&) = default;
    Float64& operator=(Float64&&) = default;

public:
    [[nodiscard]]
    QString toString() const;
    [[nodiscard]]
    QString toString(const QLocale& locale) const;

public:
    double value = 0;
};

inline Float64::Float64() = default;
inline Float64::Float64(double v) : value(v) {}

inline QString Float64::toString() const
{
    return QString::number(value, 'e', 16);
}

inline QString Float64::toString(const QLocale& locale) const
{
    return locale.toString(value, 'e', 16);
}

}

Q_DECLARE_METATYPE(Okteta::Float64)

#endif
