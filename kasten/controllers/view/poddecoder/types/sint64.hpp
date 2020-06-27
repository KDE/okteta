/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINT64_HPP
#define KASTEN_SINT64_HPP

// Qt
#include <QMetaType>
#include <QString>

struct SInt64
{
public:
    SInt64(qint64 v);
    SInt64();

public:
    QString toString() const;

public:
    qint64 value = 0;
};

inline SInt64::SInt64() = default;
inline SInt64::SInt64(qint64 v) : value(v) {}

inline QString SInt64::toString() const { return QString::number(value); }

Q_DECLARE_METATYPE(SInt64)

#endif
