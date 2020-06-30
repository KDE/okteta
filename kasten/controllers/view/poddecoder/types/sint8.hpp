/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINT8_HPP
#define KASTEN_SINT8_HPP

// Qt
#include <QMetaType>
#include <QString>

struct SInt8
{
public:
    SInt8(qint8 v);
    SInt8();

public:
    QString toString() const;

public:
    qint8 value = 0;
};

inline SInt8::SInt8() = default;
inline SInt8::SInt8(qint8 v) : value(v) {}

inline QString SInt8::toString() const { return QString::number(value); }

Q_DECLARE_METATYPE(SInt8)

#endif
