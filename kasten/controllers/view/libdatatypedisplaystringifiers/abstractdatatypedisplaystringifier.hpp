/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTDATATYPEDISPLAYSTRINGIFIER_HPP
#define OKTETA_ABSTRACTDATATYPEDISPLAYSTRINGIFIER_HPP

class QLocale;
class QString;
class QVariant;

namespace Okteta {

class AbstractDataTypeDisplayStringifier
{
public:
    virtual ~AbstractDataTypeDisplayStringifier() = default;

public:
    virtual QString displayText(const QVariant& data, const QLocale& locale, bool asHex) const = 0;
};

}

#endif
