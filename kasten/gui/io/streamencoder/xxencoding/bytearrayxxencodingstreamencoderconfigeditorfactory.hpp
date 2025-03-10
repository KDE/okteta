/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYXXENCODINGSTREAMENCODERCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYXXENCODINGSTREAMENCODERCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayXxencodingStreamEncoderConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayXxencodingStreamEncoderConfigEditorFactory();
    ByteArrayXxencodingStreamEncoderConfigEditorFactory(const ByteArrayXxencodingStreamEncoderConfigEditorFactory&) = delete;
    ByteArrayXxencodingStreamEncoderConfigEditorFactory(ByteArrayXxencodingStreamEncoderConfigEditorFactory&&) = delete;

    ~ByteArrayXxencodingStreamEncoderConfigEditorFactory() override;

    ByteArrayXxencodingStreamEncoderConfigEditorFactory& operator=(const ByteArrayXxencodingStreamEncoderConfigEditorFactory&) = delete;
    ByteArrayXxencodingStreamEncoderConfigEditorFactory& operator=(ByteArrayXxencodingStreamEncoderConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
