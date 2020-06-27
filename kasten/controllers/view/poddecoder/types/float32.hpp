/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FLOAT32_HPP
#define KASTEN_FLOAT32_HPP

// Qt
#include <QMetaType>
#include <QString>

struct Float32
{
public:
    Float32(float v);
    Float32();

public:
    QString toString() const;

public:
    float value = 0;
};

inline Float32::Float32() = default;
inline Float32::Float32(float v) : value(v) {}

inline QString Float32::toString() const
{
    return QString::number(value, 'e', 8);
}

Q_DECLARE_METATYPE(Float32)

#endif
