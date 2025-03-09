/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSTREAMENCODERFACTORY_HPP
#define KASTEN_BYTEARRAYSTREAMENCODERFACTORY_HPP

// lib
#include "oktetakastengui_export.hpp"
// Std
#include <memory>
#include <vector>

namespace Kasten {
class AbstractModelStreamEncoder;

class OKTETAKASTENGUI_EXPORT ByteArrayStreamEncoderFactory
{
public:
    ByteArrayStreamEncoderFactory() = delete;
    ByteArrayStreamEncoderFactory(const ByteArrayStreamEncoderFactory&) = delete;
    ByteArrayStreamEncoderFactory(ByteArrayStreamEncoderFactory&&) = delete;

    ByteArrayStreamEncoderFactory& operator=(const ByteArrayStreamEncoderFactory&) = delete;
    ByteArrayStreamEncoderFactory& operator=(ByteArrayStreamEncoderFactory&&) = delete;

public:
    [[nodiscard]]
    static std::vector<std::unique_ptr<AbstractModelStreamEncoder>> createStreamEncoders();

};

}

#endif
