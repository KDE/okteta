/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayzstdstreamcompressorconfigeditorfactory.hpp"

// lib
#include "bytearrayzstdstreamcompressorconfigeditor.hpp"
#include "bytearrayzstdstreamcompressor.hpp"

namespace Kasten {

ByteArrayZStdStreamCompressorConfigEditorFactory::ByteArrayZStdStreamCompressorConfigEditorFactory() = default;

ByteArrayZStdStreamCompressorConfigEditorFactory::~ByteArrayZStdStreamCompressorConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArrayZStdStreamCompressorConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* const srecStreamCompressor = qobject_cast<ByteArrayZStdStreamCompressor*>(encoder);

    if (srecStreamCompressor) {
        result = std::make_unique<ByteArrayZStdStreamCompressorConfigEditor>(srecStreamCompressor);
    }

    return result;
}

}
