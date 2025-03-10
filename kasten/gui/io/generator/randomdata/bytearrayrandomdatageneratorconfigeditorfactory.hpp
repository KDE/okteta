/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRANDOMDATAGENERATORCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYRANDOMDATAGENERATORCONFIGEDITORFACTORY_HPP

// Kasten gui
#include <Kasten/AbstractModelDataGeneratorConfigEditorFactory>

namespace Kasten {

class ByteArrayRandomDataGeneratorConfigEditorFactory : public AbstractModelDataGeneratorConfigEditorFactory
{
public:
    ByteArrayRandomDataGeneratorConfigEditorFactory();
    ByteArrayRandomDataGeneratorConfigEditorFactory(const ByteArrayRandomDataGeneratorConfigEditorFactory&) = delete;
    ByteArrayRandomDataGeneratorConfigEditorFactory(ByteArrayRandomDataGeneratorConfigEditorFactory&&) = delete;

    ~ByteArrayRandomDataGeneratorConfigEditorFactory() override;

    ByteArrayRandomDataGeneratorConfigEditorFactory& operator=(const ByteArrayRandomDataGeneratorConfigEditorFactory&) = delete;
    ByteArrayRandomDataGeneratorConfigEditorFactory& operator=(ByteArrayRandomDataGeneratorConfigEditorFactory&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractModelDataGeneratorConfigEditor> tryCreateConfigEditor(AbstractModelDataGenerator* generator) const override;
};

}

#endif
