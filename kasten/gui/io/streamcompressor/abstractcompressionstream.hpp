/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTCOMPRESSIONSTREAM_HPP
#define KASTEN_ABSTRACTCOMPRESSIONSTREAM_HPP

// Qt
#include <QString>
// Std
#include <vector>

namespace Kasten {

class AbstractByteArrayOutput;

class AbstractCompressionStream
{
protected:
    enum State {
        NotYetWritten,
        Writing,
        Finished,
        ErrorHit,
    };

    enum CompressMode {
        OngoingCompress, // TODO: better term? run(ing)?
        FinalCompress,
    };

public:
    explicit AbstractCompressionStream(AbstractByteArrayOutput* byteArrayOutput);
    virtual ~AbstractCompressionStream();

public:
    /// returns -1 on error, TODO: should be std::size_t, needs another error signalling
    [[nodiscard]]
    int writeData(const char* data, std::size_t dataSize);
    [[nodiscard]]
    bool finish();

    /// human-readable description of the last compression error
    [[nodiscard]]
    QString errorString() const;

public: // API to implement
    [[nodiscard]]
    virtual bool writeHeader();

    [[nodiscard]]
    virtual std::size_t inputBufferSizeHint() const;

protected: // API to implement
    virtual void setInputBuffer(const char* buffer, std::size_t bufferSize) = 0;

    [[nodiscard]]
    virtual bool compress(CompressMode compressMode) = 0;

protected:
    void setError(const QString& errorMessage);

    [[nodiscard]]
    State state() const;

    [[nodiscard]]
    AbstractByteArrayOutput* byteArrayOutput() const;

private:
    AbstractByteArrayOutput* const m_byteArrayOutput;

    std::vector<char> m_outputBuffer;

    State m_state = NotYetWritten;
    QString m_errorString;
};

inline QString AbstractCompressionStream::errorString() const
{
    return m_errorString;
}

inline AbstractCompressionStream::State AbstractCompressionStream::state() const
{
    return m_state;
}

inline AbstractByteArrayOutput* AbstractCompressionStream::byteArrayOutput() const
{
    return m_byteArrayOutput;
}

}

#endif
