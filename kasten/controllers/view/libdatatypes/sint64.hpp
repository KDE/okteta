/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINT64_HPP
#define KASTEN_SINT64_HPP

// Qt
#include <QLocale>
#include <QMetaType>
#include <QString>

struct SInt64
{
public:
    SInt64(qint64 v);
    SInt64(const SInt64&) = default;
    SInt64();

    ~SInt64() = default;

    SInt64& operator=(const SInt64&) = default;

public:
    QString toString() const;
    QString toString(const QLocale& locale) const;

public:
    qint64 value = 0;
};

inline SInt64::SInt64() = default;
inline SInt64::SInt64(qint64 v) : value(v) {}

inline QString SInt64::toString() const { return QString::number(value); }
inline QString SInt64::toString(const QLocale& locale) const { return locale.toString(value); }

Q_DECLARE_METATYPE(SInt64)

#endif
