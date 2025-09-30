/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraybzip2streamcompressorconfigeditorfactory.hpp"

// lib
#include "bytearraybzip2streamcompressorconfigeditor.hpp"
#include "bytearraybzip2streamcompressor.hpp"

namespace Kasten {

ByteArrayBZip2StreamCompressorConfigEditorFactory::ByteArrayBZip2StreamCompressorConfigEditorFactory() = default;

ByteArrayBZip2StreamCompressorConfigEditorFactory::~ByteArrayBZip2StreamCompressorConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArrayBZip2StreamCompressorConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* const srecStreamCompressor = qobject_cast<ByteArrayBZip2StreamCompressor*>(encoder);

    if (srecStreamCompressor) {
        result = std::make_unique<ByteArrayBZip2StreamCompressorConfigEditor>(srecStreamCompressor);
    }

    return result;
}

}
