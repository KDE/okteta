/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ZSTDCOMPRESSIONSTREAM_HPP
#define KASTEN_ZSTDCOMPRESSIONSTREAM_HPP

// lib
#include "abstractcompressionstream.hpp"
// Qt
#include <QFlags>
// zstd
#include <zstd.h>

class KLocalizedString;

namespace Kasten {

class ZStdCompressionStream : public AbstractCompressionStream
{
public:
    enum FrameFlag {
        WriteFrameContentSize = 1 << 0,
        WriteFrameChecksum = 1 << 1,
    };
    Q_DECLARE_FLAGS(FrameFlags, FrameFlag)

    static constexpr FrameFlags DefaultFrameFlags = WriteFrameContentSize;

public:
    explicit ZStdCompressionStream(AbstractByteArrayOutput* byteArrayOutput, int compressionLevel = ZSTD_CLEVEL_DEFAULT, FrameFlags frameFlags = DefaultFrameFlags);
    ~ZStdCompressionStream() override;

protected: // AbstractCompressionStream API
    void setInputBuffer(const char* buffer, std::size_t bufferSize) override;

    [[nodiscard]]
    bool compress(CompressMode compressMode) override;

    [[nodiscard]]
    std::size_t inputBufferSizeHint() const override;

private:
    void setZStdError(const KLocalizedString& errorMessage, size_t zStdErrorCode);

private:
    std::vector<char> m_outputBuffer;

    ::ZSTD_inBuffer m_inputBufferData;
    ::ZSTD_outBuffer m_outputBufferData;

    ::ZSTD_CCtx* m_compressionStreamContext = nullptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ZStdCompressionStream::FrameFlags)

}

#endif
