/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYGZIPSTREAMCOMPRESSORCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYGZIPSTREAMCOMPRESSORCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayGZipStreamCompressorConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayGZipStreamCompressorConfigEditorFactory();
    ByteArrayGZipStreamCompressorConfigEditorFactory(const ByteArrayGZipStreamCompressorConfigEditorFactory&) = delete;
    ByteArrayGZipStreamCompressorConfigEditorFactory(ByteArrayGZipStreamCompressorConfigEditorFactory&&) = delete;

    ~ByteArrayGZipStreamCompressorConfigEditorFactory() override;

    ByteArrayGZipStreamCompressorConfigEditorFactory& operator=(const ByteArrayGZipStreamCompressorConfigEditorFactory&) = delete;
    ByteArrayGZipStreamCompressorConfigEditorFactory& operator=(ByteArrayGZipStreamCompressorConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
