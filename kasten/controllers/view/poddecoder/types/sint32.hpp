/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINT32_HPP
#define KASTEN_SINT32_HPP

// Qt
#include <QMetaType>
#include <QString>

struct SInt32
{
public:
    SInt32(qint32 v);
    SInt32();

public:
    QString toString() const;

public:
    qint32 value = 0;
};

inline SInt32::SInt32() = default;
inline SInt32::SInt32(qint32 v) : value(v) {}

inline QString SInt32::toString() const { return QString::number(value); }

Q_DECLARE_METATYPE(SInt32)

#endif
