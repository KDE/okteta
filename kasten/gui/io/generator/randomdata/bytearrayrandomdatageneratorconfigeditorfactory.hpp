/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRANDOMDATAGENERATORCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYRANDOMDATAGENERATORCONFIGEDITORFACTORY_HPP

// lib
#include "oktetakastengui_export.hpp"
// Kasten gui
#include <Kasten/AbstractModelDataGeneratorConfigEditorFactory>

namespace Kasten {

class ByteArrayRandomDataGeneratorConfigEditorFactory : public AbstractModelDataGeneratorConfigEditorFactory
{
public:
    ByteArrayRandomDataGeneratorConfigEditorFactory();

    ~ByteArrayRandomDataGeneratorConfigEditorFactory() override;

public:
    AbstractModelDataGeneratorConfigEditor* tryCreateConfigEditor(AbstractModelDataGenerator* generator) const override;
};

}

#endif
