/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODSUMBYTEARRAYCHECKSUMPARAMETERSET_HPP
#define KASTEN_MODSUMBYTEARRAYCHECKSUMPARAMETERSET_HPP

// lib
#include "abstractbytearraychecksumparameterset.hpp"
#include "endianness.hpp"

class ModSumByteArrayChecksumParameterSet : public AbstractByteArrayChecksumParameterSet
{
public:
    ModSumByteArrayChecksumParameterSet();
    ~ModSumByteArrayChecksumParameterSet() override;

public: // AbstractByteArrayChecksumParameterSet API
    const char* id() const override;

public:
    void setEndianness(Endianness endianness);

public:
    Endianness endianness() const;

private:
    Endianness mEndianness = ThisMachineEndianness;
};

#endif
