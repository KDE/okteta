/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCHARSSTREAMENCODERCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYCHARSSTREAMENCODERCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayCharsStreamEncoderConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayCharsStreamEncoderConfigEditorFactory();
    ByteArrayCharsStreamEncoderConfigEditorFactory(const ByteArrayCharsStreamEncoderConfigEditorFactory&) = delete;
    ByteArrayCharsStreamEncoderConfigEditorFactory(ByteArrayCharsStreamEncoderConfigEditorFactory&&) = delete;

    ~ByteArrayCharsStreamEncoderConfigEditorFactory() override;

    ByteArrayCharsStreamEncoderConfigEditorFactory& operator=(const ByteArrayCharsStreamEncoderConfigEditorFactory&) = delete;
    ByteArrayCharsStreamEncoderConfigEditorFactory& operator=(ByteArrayCharsStreamEncoderConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
