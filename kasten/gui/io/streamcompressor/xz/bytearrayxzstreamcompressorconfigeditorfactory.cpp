/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayxzstreamcompressorconfigeditorfactory.hpp"

// lib
#include "bytearrayxzstreamcompressorconfigeditor.hpp"
#include "bytearrayxzstreamcompressor.hpp"

namespace Kasten {

ByteArrayXzStreamCompressorConfigEditorFactory::ByteArrayXzStreamCompressorConfigEditorFactory() = default;

ByteArrayXzStreamCompressorConfigEditorFactory::~ByteArrayXzStreamCompressorConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArrayXzStreamCompressorConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* const srecStreamCompressor = qobject_cast<ByteArrayXzStreamCompressor*>(encoder);

    if (srecStreamCompressor) {
        result = std::make_unique<ByteArrayXzStreamCompressorConfigEditor>(srecStreamCompressor);
    }

    return result;
}

}
