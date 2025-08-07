/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYBZIP2STREAMCOMPRESSORCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYBZIP2STREAMCOMPRESSORCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayBZip2StreamCompressorConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayBZip2StreamCompressorConfigEditorFactory();
    ByteArrayBZip2StreamCompressorConfigEditorFactory(const ByteArrayBZip2StreamCompressorConfigEditorFactory&) = delete;
    ByteArrayBZip2StreamCompressorConfigEditorFactory(ByteArrayBZip2StreamCompressorConfigEditorFactory&&) = delete;

    ~ByteArrayBZip2StreamCompressorConfigEditorFactory() override;

    ByteArrayBZip2StreamCompressorConfigEditorFactory& operator=(const ByteArrayBZip2StreamCompressorConfigEditorFactory&) = delete;
    ByteArrayBZip2StreamCompressorConfigEditorFactory& operator=(ByteArrayBZip2StreamCompressorConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
