/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Lars Maier <lars.maier@tefax.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CRC64BYTEARRAYCHECKSUMPARAMETERSET_HPP
#define KASTEN_CRC64BYTEARRAYCHECKSUMPARAMETERSET_HPP

// lib
#include "abstractbytearraychecksumparameterset.hpp"

enum class Crc64Variant
{
    ECMA182 = 0,
    ISO3309 = 1,
};

class Crc64ByteArrayChecksumParameterSet : public AbstractByteArrayChecksumParameterSet
{
public:
    Crc64ByteArrayChecksumParameterSet();
    ~Crc64ByteArrayChecksumParameterSet() override;

public: // AbstractByteArrayChecksumParameterSet API
    const char* id() const override;

public:
    void setCrc64Variant(Crc64Variant variant);

public:
    Crc64Variant crc64Variant() const;

private:
    Crc64Variant mVariant = Crc64Variant::ECMA182;
};

#endif
