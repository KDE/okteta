/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVALUESSTREAMENCODERCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYVALUESSTREAMENCODERCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayValuesStreamEncoderConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayValuesStreamEncoderConfigEditorFactory();
    ByteArrayValuesStreamEncoderConfigEditorFactory(const ByteArrayValuesStreamEncoderConfigEditorFactory&) = delete;
    ByteArrayValuesStreamEncoderConfigEditorFactory(ByteArrayValuesStreamEncoderConfigEditorFactory&&) = delete;

    ~ByteArrayValuesStreamEncoderConfigEditorFactory() override;

    ByteArrayValuesStreamEncoderConfigEditorFactory& operator=(const ByteArrayValuesStreamEncoderConfigEditorFactory&) = delete;
    ByteArrayValuesStreamEncoderConfigEditorFactory& operator=(ByteArrayValuesStreamEncoderConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif
