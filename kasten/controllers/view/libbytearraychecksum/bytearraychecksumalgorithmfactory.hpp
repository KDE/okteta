/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCHECKSUMALGORITHMFACTORY_HPP
#define KASTEN_BYTEARRAYCHECKSUMALGORITHMFACTORY_HPP

// Std
#include <memory>
#include <vector>

class AbstractByteArrayChecksumAlgorithm;

class ByteArrayChecksumAlgorithmFactory
{
public:
    ByteArrayChecksumAlgorithmFactory() = delete;
    ByteArrayChecksumAlgorithmFactory(const ByteArrayChecksumAlgorithmFactory&) = delete;
    ByteArrayChecksumAlgorithmFactory(ByteArrayChecksumAlgorithmFactory&&) = delete;

    ByteArrayChecksumAlgorithmFactory& operator=(const ByteArrayChecksumAlgorithmFactory&) = delete;
    ByteArrayChecksumAlgorithmFactory& operator=(ByteArrayChecksumAlgorithmFactory&&) = delete;

public:
    [[nodiscard]]
    static std::vector<std::unique_ptr<AbstractByteArrayChecksumAlgorithm>> createAlgorithms();

};

#endif
