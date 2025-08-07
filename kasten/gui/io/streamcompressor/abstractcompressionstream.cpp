/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractcompressionstream.hpp"

// lib
#include "abstractbytearrayoutput.hpp"
// Std
#include <cstdio>

namespace Kasten {

AbstractCompressionStream::AbstractCompressionStream(AbstractByteArrayOutput* byteArrayOutput)
    : m_byteArrayOutput(byteArrayOutput)
{
}

AbstractCompressionStream::~AbstractCompressionStream()
{
    std::ignore = finish();
}

void AbstractCompressionStream::setError(const QString& errorMessage)
{
    m_state = ErrorHit;

    m_errorString = errorMessage;
}

bool AbstractCompressionStream::writeHeader()
{
    return true;
}

int AbstractCompressionStream::writeData(const char* data, std::size_t dataSize)
{
    if (dataSize < 1) {
        return 0;
    }

    if (m_state == ErrorHit) {
        return -1;
    }

    setInputBuffer(data, dataSize);

    const bool compressResult = compress(OngoingCompress);

    if (!compressResult) {
        return -1;
    }

    m_state = Writing;

    return dataSize;
}

bool AbstractCompressionStream::finish()
{
    if (m_state == Writing) {
        const bool compressResult = compress(FinalCompress);
        if (!compressResult) {
            return false;
        }
    } else {
        if (m_state != NotYetWritten) {
            return false;
        }
    }

    m_state = Finished;

    return true;
}

std::size_t AbstractCompressionStream::inputBufferSizeHint() const
{
    // using stdio default buffer size TODO: better choice?
    return BUFSIZ;
}

}
