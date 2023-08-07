/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "char8.hpp"

// TODO: add toDisplayString variant, tag /x and undefined for display formatting
QString Char8::toString() const
{
    if (character.isUndefined()) {
        return QString(Okteta::DefaultUndefinedChar);
    }

    switch (character.unicode()) {
    case '\0': return QStringLiteral("\\0");
    case '\a': return QStringLiteral("\\a");
    case '\b': return QStringLiteral("\\b");
    case '\f': return QStringLiteral("\\f");
    case '\n': return QStringLiteral("\\n");
    case '\r': return QStringLiteral("\\r");
    case '\t': return QStringLiteral("\\t");
    case '\v': return QStringLiteral("\\v");
    default: break;
    }

    return QString(character);
}
