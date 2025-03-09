/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSRECSTREAMENCODERCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYSRECSTREAMENCODERCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArraySRecStreamEncoderConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArraySRecStreamEncoderConfigEditorFactory();
    ByteArraySRecStreamEncoderConfigEditorFactory(const ByteArraySRecStreamEncoderConfigEditorFactory&) = delete;
    ByteArraySRecStreamEncoderConfigEditorFactory(ByteArraySRecStreamEncoderConfigEditorFactory&&) = delete;

    ~ByteArraySRecStreamEncoderConfigEditorFactory() override;

    ByteArraySRecStreamEncoderConfigEditorFactory& operator=(const ByteArraySRecStreamEncoderConfigEditorFactory&) = delete;
    ByteArraySRecStreamEncoderConfigEditorFactory& operator=(ByteArraySRecStreamEncoderConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
