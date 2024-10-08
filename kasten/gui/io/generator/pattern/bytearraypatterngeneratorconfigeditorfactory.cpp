/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraypatterngeneratorconfigeditorfactory.hpp"

// lib
#include "bytearraypatterngeneratorconfigeditor.hpp"
#include "bytearraypatterngenerator.hpp"

namespace Kasten {

ByteArrayPatternGeneratorConfigEditorFactory::ByteArrayPatternGeneratorConfigEditorFactory() = default;

ByteArrayPatternGeneratorConfigEditorFactory::~ByteArrayPatternGeneratorConfigEditorFactory() = default;

std::unique_ptr<AbstractModelDataGeneratorConfigEditor> ByteArrayPatternGeneratorConfigEditorFactory::tryCreateConfigEditor(AbstractModelDataGenerator* generator) const
{
    std::unique_ptr<AbstractModelDataGeneratorConfigEditor> result;

    auto* patternGenerator = qobject_cast<ByteArrayPatternGenerator*>(generator);

    if (patternGenerator) {
        result = std::make_unique<ByteArrayPatternGeneratorConfigEditor>(patternGenerator);
    }

    return result;
}

}
