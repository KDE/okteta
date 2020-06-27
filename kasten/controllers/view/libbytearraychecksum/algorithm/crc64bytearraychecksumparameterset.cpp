/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Lars Maier <lars.maier@tefax.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "crc64bytearraychecksumparameterset.hpp"

Crc64ByteArrayChecksumParameterSet::Crc64ByteArrayChecksumParameterSet() = default;

Crc64ByteArrayChecksumParameterSet::~Crc64ByteArrayChecksumParameterSet() = default;

const char* Crc64ByteArrayChecksumParameterSet::id() const { return "Crc64"; }

Crc64Variant Crc64ByteArrayChecksumParameterSet::crc64Variant() const { return mVariant; }

void Crc64ByteArrayChecksumParameterSet::setCrc64Variant(Crc64Variant variant) { mVariant = variant; }
