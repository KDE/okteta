/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FLOAT64_HPP
#define KASTEN_FLOAT64_HPP

// Qt
#include <QMetaType>
#include <QString>

struct Float64
{
public:
    Float64(double v);
    Float64();

public:
    QString toString() const;

public:
    double value = 0;
};

inline Float64::Float64() = default;
inline Float64::Float64(double v) : value(v) {}

inline QString Float64::toString() const
{
    return QString::number(value, 'e', 16);
}

Q_DECLARE_METATYPE(Float64)

#endif
