/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELDATAGENERATORCONFIGEDITORFACTORY_HPP
#define KASTEN_ABSTRACTMODELDATAGENERATORCONFIGEDITORFACTORY_HPP

// lib
#include "kastengui_export.hpp"
// Std
#include <memory>

namespace Kasten {

class AbstractModelDataGeneratorConfigEditor;
class AbstractModelDataGenerator;

class KASTENGUI_EXPORT AbstractModelDataGeneratorConfigEditorFactory
{
public:
    virtual ~AbstractModelDataGeneratorConfigEditorFactory();

public:
    [[nodiscard]]
    virtual std::unique_ptr<AbstractModelDataGeneratorConfigEditor> tryCreateConfigEditor(AbstractModelDataGenerator* generator) const = 0;
};

inline AbstractModelDataGeneratorConfigEditorFactory::~AbstractModelDataGeneratorConfigEditorFactory() = default;

}

#endif
