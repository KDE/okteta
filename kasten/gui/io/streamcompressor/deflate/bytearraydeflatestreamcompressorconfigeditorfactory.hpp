/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYDEFLATESTREAMCOMPRESSORCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYDEFLATESTREAMCOMPRESSORCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayDeflateStreamCompressorConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayDeflateStreamCompressorConfigEditorFactory();
    ByteArrayDeflateStreamCompressorConfigEditorFactory(const ByteArrayDeflateStreamCompressorConfigEditorFactory&) = delete;
    ByteArrayDeflateStreamCompressorConfigEditorFactory(ByteArrayDeflateStreamCompressorConfigEditorFactory&&) = delete;

    ~ByteArrayDeflateStreamCompressorConfigEditorFactory() override;

    ByteArrayDeflateStreamCompressorConfigEditorFactory& operator=(const ByteArrayDeflateStreamCompressorConfigEditorFactory&) = delete;
    ByteArrayDeflateStreamCompressorConfigEditorFactory& operator=(ByteArrayDeflateStreamCompressorConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
