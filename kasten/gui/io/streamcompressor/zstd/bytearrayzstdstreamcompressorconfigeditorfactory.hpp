/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYZSTDSTREAMCOMPRESSORCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYZSTDSTREAMCOMPRESSORCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayZStdStreamCompressorConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayZStdStreamCompressorConfigEditorFactory();
    ByteArrayZStdStreamCompressorConfigEditorFactory(const ByteArrayZStdStreamCompressorConfigEditorFactory&) = delete;
    ByteArrayZStdStreamCompressorConfigEditorFactory(ByteArrayZStdStreamCompressorConfigEditorFactory&&) = delete;

    ~ByteArrayZStdStreamCompressorConfigEditorFactory() override;

    ByteArrayZStdStreamCompressorConfigEditorFactory& operator=(const ByteArrayZStdStreamCompressorConfigEditorFactory&) = delete;
    ByteArrayZStdStreamCompressorConfigEditorFactory& operator=(ByteArrayZStdStreamCompressorConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
