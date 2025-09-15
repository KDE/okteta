/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "zstdcompressionstream.hpp"

// lib
#include "abstractbytearrayoutput.hpp"
#include <logging.hpp>
// KF
#include <KLocalizedString>

namespace Kasten {

ZStdCompressionStream::ZStdCompressionStream(AbstractByteArrayOutput* byteArrayOutput,
                                             int compressionLevel,
                                             FrameFlags frameFlags)
    : AbstractCompressionStream(byteArrayOutput)
{
    m_compressionStreamContext = ::ZSTD_createCCtx();
    if (!m_compressionStreamContext) {
        setError(i18n("Could not create zstd stream context."));
        return;
    }
    size_t setContextParameterResult = ::ZSTD_CCtx_setParameter(m_compressionStreamContext, ::ZSTD_c_compressionLevel, compressionLevel);
    if (::ZSTD_isError(setContextParameterResult)) {
        setZStdError(ki18n("Internal zstd error on setup: %1"), setContextParameterResult);
        return;
    }
    const int contentSizeFlag = frameFlags.testFlag(WriteFrameContentSize) ? 1 : 0;
    setContextParameterResult = ::ZSTD_CCtx_setParameter(m_compressionStreamContext, ::ZSTD_c_contentSizeFlag, contentSizeFlag);
    if (::ZSTD_isError(setContextParameterResult)) {
        setZStdError(ki18n("Internal zstd error on setup: %1"), setContextParameterResult);
        return;
    }
    const int checksumFlag = frameFlags.testFlag(WriteFrameChecksum) ? 1 : 0;
    // TODO: does this really work with the streaming API?
    // zstd:examples/streaming_compression.c sets it, but API docs are not clear here on first read
    setContextParameterResult = ::ZSTD_CCtx_setParameter(m_compressionStreamContext, ::ZSTD_c_checksumFlag, checksumFlag);
    if (::ZSTD_isError(setContextParameterResult)) {
        setZStdError(ki18n("Internal zstd error on setup: %1"), setContextParameterResult);
        return;
    }

    const size_t buffOutSize = ::ZSTD_CStreamOutSize();
    m_outputBuffer.resize(buffOutSize);
}

ZStdCompressionStream::~ZStdCompressionStream()
{
    // TODO: how to deal with NotYetWritten?
    std::ignore = finish();

    std::ignore = ::ZSTD_freeCCtx(m_compressionStreamContext);
}

void ZStdCompressionStream::setZStdError(const KLocalizedString& errorMessage, size_t zStdErrorCode)
{
    const char* const zStdErrorString = ::ZSTD_getErrorName(zStdErrorCode);

    const QString argument = zStdErrorString ?
        QString::fromLocal8Bit(zStdErrorString) :
        i18n("Unknown zstd error, code %1", QString::number(zStdErrorCode));

    setError(errorMessage.subs(argument).toString());
}

bool ZStdCompressionStream::compress(CompressMode compressMode)
{
    AbstractByteArrayOutput* byteArrayOutput = this->byteArrayOutput();

    const bool isFinishing = (compressMode == FinalCompress);
    const ::ZSTD_EndDirective endDirective = isFinishing ? ::ZSTD_e_end : ::ZSTD_e_continue;
    bool isInputHandled;
    do {
        m_outputBufferData.dst = m_outputBuffer.data();
        m_outputBufferData.size = m_outputBuffer.size();
        m_outputBufferData.pos = 0;

        const size_t remaining = ::ZSTD_compressStream2(m_compressionStreamContext, &m_outputBufferData , &m_inputBufferData, endDirective);
        if (::ZSTD_isError(remaining)) {
            setZStdError(ki18n("Internal zstd error when compressing: %1"), remaining);
            return false;
        }

        const bool isWritten = byteArrayOutput->writeData(m_outputBuffer.data(), m_outputBufferData.pos);

        if (!isWritten) {
            setError(i18n("Error writing to output: %1", byteArrayOutput->errorString()));

            return false; // TODO; other error code?
        }

        isInputHandled = isFinishing ? (remaining == 0) : (m_inputBufferData.pos == m_inputBufferData.size);
    } while (!isInputHandled); //  as long as the full output buffer is used

    return true;
}

void ZStdCompressionStream::setInputBuffer(const char* buffer, std::size_t bufferSize)
{
    m_inputBufferData.src = buffer;
    m_inputBufferData.size = bufferSize;
    m_inputBufferData.pos = 0;
}

std::size_t ZStdCompressionStream::inputBufferSizeHint() const
{
    return ::ZSTD_CStreamInSize();
}

}
