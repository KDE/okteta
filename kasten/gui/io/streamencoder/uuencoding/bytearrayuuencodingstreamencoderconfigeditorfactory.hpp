/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYUUENCODINGSTREAMENCODERCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYUUENCODINGSTREAMENCODERCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayUuencodingStreamEncoderConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayUuencodingStreamEncoderConfigEditorFactory();
    ByteArrayUuencodingStreamEncoderConfigEditorFactory(const ByteArrayUuencodingStreamEncoderConfigEditorFactory&) = delete;
    ByteArrayUuencodingStreamEncoderConfigEditorFactory(ByteArrayUuencodingStreamEncoderConfigEditorFactory&&) = delete;

    ~ByteArrayUuencodingStreamEncoderConfigEditorFactory() override;

    ByteArrayUuencodingStreamEncoderConfigEditorFactory& operator=(const ByteArrayUuencodingStreamEncoderConfigEditorFactory&) = delete;
    ByteArrayUuencodingStreamEncoderConfigEditorFactory& operator=(ByteArrayUuencodingStreamEncoderConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
