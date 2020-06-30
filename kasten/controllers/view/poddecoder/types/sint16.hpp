/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINT16_HPP
#define KASTEN_SINT16_HPP

// Qt
#include <QMetaType>
#include <QString>

struct SInt16
{
public:
    SInt16(qint16 v);
    SInt16();

public:
    QString toString() const;

public:
    qint16 value = 0;
};

inline SInt16::SInt16() = default;
inline SInt16::SInt16(qint16 v) : value(v) {}

inline QString SInt16::toString() const { return QString::number(value); }

Q_DECLARE_METATYPE(SInt16)

#endif
