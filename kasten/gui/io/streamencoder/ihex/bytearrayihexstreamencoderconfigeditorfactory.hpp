/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYIHEXSTREAMENCODERCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYIHEXSTREAMENCODERCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayIHexStreamEncoderConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayIHexStreamEncoderConfigEditorFactory();
    ByteArrayIHexStreamEncoderConfigEditorFactory(const ByteArrayIHexStreamEncoderConfigEditorFactory&) = delete;
    ByteArrayIHexStreamEncoderConfigEditorFactory(ByteArrayIHexStreamEncoderConfigEditorFactory&&) = delete;

    ~ByteArrayIHexStreamEncoderConfigEditorFactory() override;

    ByteArrayIHexStreamEncoderConfigEditorFactory& operator=(const ByteArrayIHexStreamEncoderConfigEditorFactory&) = delete;
    ByteArrayIHexStreamEncoderConfigEditorFactory& operator=(ByteArrayIHexStreamEncoderConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
