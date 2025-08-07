/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYOUTPUT_HPP
#define KASTEN_ABSTRACTBYTEARRAYOUTPUT_HPP

// Std
#include <cstddef>

class QString;

namespace Kasten {

// TODO: consider making AbstractModelStreamEncoder::encodeToStream use this instead of QIODevice
// TODO: better terms than "write" and "output"?
class AbstractByteArrayOutput
{
public:
    virtual ~AbstractByteArrayOutput();

public: // API to be implemented
    [[nodiscard]]
    virtual QString errorString() const = 0;

    /// returns @false on error
    [[nodiscard]]
    virtual bool writeData(const char* data, std::size_t dataSize) = 0;
};

inline AbstractByteArrayOutput::~AbstractByteArrayOutput() = default;

}

#endif
