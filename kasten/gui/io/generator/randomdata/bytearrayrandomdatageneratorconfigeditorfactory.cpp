/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrandomdatageneratorconfigeditorfactory.hpp"

// lib
#include "bytearrayrandomdatageneratorconfigeditor.hpp"
#include "bytearrayrandomdatagenerator.hpp"

namespace Kasten {

ByteArrayRandomDataGeneratorConfigEditorFactory::ByteArrayRandomDataGeneratorConfigEditorFactory() = default;

ByteArrayRandomDataGeneratorConfigEditorFactory::~ByteArrayRandomDataGeneratorConfigEditorFactory() = default;

std::unique_ptr<AbstractModelDataGeneratorConfigEditor> ByteArrayRandomDataGeneratorConfigEditorFactory::tryCreateConfigEditor(AbstractModelDataGenerator* generator) const
{
    std::unique_ptr<AbstractModelDataGeneratorConfigEditor> result;

    auto* randomDataGenerator = qobject_cast<ByteArrayRandomDataGenerator*>(generator);

    if (randomDataGenerator) {
        result = std::make_unique<ByteArrayRandomDataGeneratorConfigEditor>(randomDataGenerator);
    }

    return result;
}

}
