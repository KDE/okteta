/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Lars Maier <lars.maier@tefax.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CRC64BYTEARRAYCHECKSUMALGORITHM_HPP
#define KASTEN_CRC64BYTEARRAYCHECKSUMALGORITHM_HPP

//
#include "crc64bytearraychecksumparameterset.hpp"
//
#include <abstractbytearraychecksumalgorithm.hpp>

class Crc64ByteArrayChecksumAlgorithm : public AbstractByteArrayChecksumAlgorithm
{
    Q_OBJECT

public:
    Crc64ByteArrayChecksumAlgorithm();
    ~Crc64ByteArrayChecksumAlgorithm() override;

public: // AbstractByteArrayChecksumAlgorithm API
    bool calculateChecksum(QString* result, const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const override;
    AbstractByteArrayChecksumParameterSet* parameterSet() override;

private:
    Crc64ByteArrayChecksumParameterSet mParameterSet;
};

#endif
