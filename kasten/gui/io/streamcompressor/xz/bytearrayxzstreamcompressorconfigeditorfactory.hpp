/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYXZSTREAMCOMPRESSORCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYXZSTREAMCOMPRESSORCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayXzStreamCompressorConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayXzStreamCompressorConfigEditorFactory();
    ByteArrayXzStreamCompressorConfigEditorFactory(const ByteArrayXzStreamCompressorConfigEditorFactory&) = delete;
    ByteArrayXzStreamCompressorConfigEditorFactory(ByteArrayXzStreamCompressorConfigEditorFactory&&) = delete;

    ~ByteArrayXzStreamCompressorConfigEditorFactory() override;

    ByteArrayXzStreamCompressorConfigEditorFactory& operator=(const ByteArrayXzStreamCompressorConfigEditorFactory&) = delete;
    ByteArrayXzStreamCompressorConfigEditorFactory& operator=(ByteArrayXzStreamCompressorConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
