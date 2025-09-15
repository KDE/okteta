/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "versionviewtoolfactory.hpp"

// lib
#include "versionviewtool.hpp"

namespace Kasten {

VersionViewToolFactory::VersionViewToolFactory() = default;

VersionViewToolFactory::~VersionViewToolFactory() = default;

std::unique_ptr<AbstractTool> VersionViewToolFactory::create() const
{
    return std::make_unique<VersionViewTool>();
}

}
