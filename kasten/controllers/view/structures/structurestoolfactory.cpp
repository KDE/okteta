/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structurestoolfactory.hpp"

// lib
#include "structurestool.hpp"

namespace Kasten {

StructuresToolFactory::StructuresToolFactory() = default;

StructuresToolFactory::~StructuresToolFactory() = default;

AbstractTool* StructuresToolFactory::create() const
{
    return new StructuresTool();
}

}
