/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "xzcompressionstream.hpp"

// lib
#include "abstractbytearrayoutput.hpp"
#include <logging.hpp>
// KF
#include <KLocalizedString>

namespace Kasten {

XzCompressionStream::XzCompressionStream(AbstractByteArrayOutput* byteArrayOutput,
                                         int compressionPresetLevel, bool useExtremeLevel, ::lzma_check check)
    : AbstractCompressionStream(byteArrayOutput)
{
    uint32_t preset = compressionPresetLevel;
    if (useExtremeLevel) {
        preset |= LZMA_PRESET_EXTREME;
    }
    const ::lzma_ret initResult = ::lzma_easy_encoder(&m_lzmaStream, preset, check);

    if (initResult != ::LZMA_OK) {
        setLzmaError(ki18n("Could not initialize lzma stream."), initResult);
    }
}

XzCompressionStream::~XzCompressionStream()
{
    // TODO: how to deal with NotYetWritten?
    std::ignore = finish();

    ::lzma_end(&m_lzmaStream);
}

void XzCompressionStream::setLzmaError(const KLocalizedString& errorMessage, ::lzma_ret returnCode)
{
    const char* const lzmaErrorString = nullptr; // TODO

    const QString argument = lzmaErrorString ?
        QString::fromLocal8Bit(lzmaErrorString) :
        i18n("Unknown lzma error, code %1", returnCode);

    setError(errorMessage.subs(argument).toString());
}

bool XzCompressionStream::compress(CompressMode compressMode)
{
    AbstractByteArrayOutput* byteArrayOutput = this->byteArrayOutput();

    ::lzma_ret codeResult;
    const ::lzma_action codeAction = (compressMode == FinalCompress) ? ::LZMA_FINISH : ::LZMA_RUN;
    do {
        m_lzmaStream.next_out = m_outputBuffer.data();
        m_lzmaStream.avail_out = m_outputBuffer.size();

        codeResult = ::lzma_code(&m_lzmaStream, codeAction);

        if (codeResult != ::LZMA_OK) {
            if ((compressMode == FinalCompress) && (codeResult != ::LZMA_STREAM_END)) {
                setLzmaError(ki18n("Internal lzma error when compressing: %1"), codeResult);
                return false;
            }
        }

        const int usedOutputSize = m_outputBuffer.size() - m_lzmaStream.avail_out;

        const bool isWritten = byteArrayOutput->writeData(reinterpret_cast<char*>(m_outputBuffer.data()), usedOutputSize);

        if (!isWritten) {
            setError(i18n("Error writing to output: %1", byteArrayOutput->errorString()));

            return false;
        }
    } while (m_lzmaStream.avail_out == 0); //  as long as the full output buffer is used

    const int expectedXzResult = (compressMode == FinalCompress) ? ::LZMA_STREAM_END : ::LZMA_OK;
    return (codeResult == expectedXzResult);
;
}

void XzCompressionStream::setInputBuffer(const char* buffer, std::size_t bufferSize)
{
    m_lzmaStream.next_in = reinterpret_cast<const uint8_t*>(buffer);
    m_lzmaStream.avail_in = bufferSize;
}

}
