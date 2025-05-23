/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSOURCECODESTREAMENCODERCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYSOURCECODESTREAMENCODERCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArraySourceCodeStreamEncoderConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArraySourceCodeStreamEncoderConfigEditorFactory();
    ByteArraySourceCodeStreamEncoderConfigEditorFactory(const ByteArraySourceCodeStreamEncoderConfigEditorFactory&) = delete;
    ByteArraySourceCodeStreamEncoderConfigEditorFactory(ByteArraySourceCodeStreamEncoderConfigEditorFactory&&) = delete;

    ~ByteArraySourceCodeStreamEncoderConfigEditorFactory() override;

    ByteArraySourceCodeStreamEncoderConfigEditorFactory& operator=(const ByteArraySourceCodeStreamEncoderConfigEditorFactory&) = delete;
    ByteArraySourceCodeStreamEncoderConfigEditorFactory& operator=(ByteArraySourceCodeStreamEncoderConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
