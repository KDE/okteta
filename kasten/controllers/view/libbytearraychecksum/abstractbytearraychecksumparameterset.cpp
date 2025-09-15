/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearraychecksumparameterset.hpp"

class AbstractByteArrayChecksumParameterSetPrivate
{
};

AbstractByteArrayChecksumParameterSet::AbstractByteArrayChecksumParameterSet()
    : d(std::make_unique<AbstractByteArrayChecksumParameterSetPrivate>())
{}

AbstractByteArrayChecksumParameterSet::~AbstractByteArrayChecksumParameterSet() = default;
