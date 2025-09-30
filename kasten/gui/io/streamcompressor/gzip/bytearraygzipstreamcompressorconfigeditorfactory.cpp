/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraygzipstreamcompressorconfigeditorfactory.hpp"

// lib
#include "bytearraygzipstreamcompressorconfigeditor.hpp"
#include "bytearraygzipstreamcompressor.hpp"

namespace Kasten {

ByteArrayGZipStreamCompressorConfigEditorFactory::ByteArrayGZipStreamCompressorConfigEditorFactory() = default;

ByteArrayGZipStreamCompressorConfigEditorFactory::~ByteArrayGZipStreamCompressorConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArrayGZipStreamCompressorConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* const srecStreamCompressor = qobject_cast<ByteArrayGZipStreamCompressor*>(encoder);

    if (srecStreamCompressor) {
        result = std::make_unique<ByteArrayGZipStreamCompressorConfigEditor>(srecStreamCompressor);
    }

    return result;
}

}
