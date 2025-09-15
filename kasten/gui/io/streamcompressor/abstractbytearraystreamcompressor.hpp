/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYSTREAMCOMPRESSOR_HPP
#define KASTEN_ABSTRACTBYTEARRAYSTREAMCOMPRESSOR_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Std
#include <memory>

namespace Kasten {

class AbstractCompressionStream;
class AbstractByteArrayOutput;

// subclass for now of AbstractByteArrayStreamEncoder
// TODO: reason if compressing is encoding, or if this needs another category, even more in UI
class AbstractByteArrayStreamCompressor : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    AbstractByteArrayStreamCompressor(const QString& remoteTypeName, const QString& remoteMimeType,
                                      const QString& remoteClipboardMimeType = QString());

    ~AbstractByteArrayStreamCompressor() override;

protected: // AbstractByteArrayStreamEncoder API
    [[nodiscard]]
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

protected: // API to implement
    [[nodiscard]]
    virtual std::unique_ptr<AbstractCompressionStream> createCompressionStream(AbstractByteArrayOutput* byteArrayOutput) = 0;
};

}

#endif
