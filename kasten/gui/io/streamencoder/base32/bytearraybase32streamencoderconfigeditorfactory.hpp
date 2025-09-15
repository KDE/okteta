/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYBASE32STREAMENCODERCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYBASE32STREAMENCODERCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayBase32StreamEncoderConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayBase32StreamEncoderConfigEditorFactory();
    ByteArrayBase32StreamEncoderConfigEditorFactory(const ByteArrayBase32StreamEncoderConfigEditorFactory&) = delete;
    ByteArrayBase32StreamEncoderConfigEditorFactory(ByteArrayBase32StreamEncoderConfigEditorFactory&&) = delete;

    ~ByteArrayBase32StreamEncoderConfigEditorFactory() override;

    ByteArrayBase32StreamEncoderConfigEditorFactory& operator=(const ByteArrayBase32StreamEncoderConfigEditorFactory&) = delete;
    ByteArrayBase32StreamEncoderConfigEditorFactory& operator=(ByteArrayBase32StreamEncoderConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
