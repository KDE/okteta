/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2019 Lars Maier <lars.maier@tefax.net>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
