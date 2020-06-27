/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "versionviewtoolfactory.hpp"

// lib
#include "versionviewtool.hpp"

namespace Kasten {

VersionViewToolFactory::VersionViewToolFactory() = default;

VersionViewToolFactory::~VersionViewToolFactory() = default;

AbstractTool* VersionViewToolFactory::create() const
{
    return new VersionViewTool();
}

}
