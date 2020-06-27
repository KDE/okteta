/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_NOBYTEARRAYCHECKSUMPARAMETERSET_HPP
#define KASTEN_NOBYTEARRAYCHECKSUMPARAMETERSET_HPP

// lib
#include "abstractbytearraychecksumparameterset.hpp"

class NoByteArrayChecksumParameterSet : public AbstractByteArrayChecksumParameterSet
{
public:
    NoByteArrayChecksumParameterSet();
    ~NoByteArrayChecksumParameterSet() override;

public: // AbstractByteArrayChecksumParameterSet API
    const char* id() const override;
};

#endif
