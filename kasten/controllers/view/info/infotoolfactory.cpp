/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "infotoolfactory.hpp"

// lib
#include "infotool.hpp"

namespace Kasten {

InfoToolFactory::InfoToolFactory() = default;

InfoToolFactory::~InfoToolFactory() = default;

std::unique_ptr<AbstractTool> InfoToolFactory::create() const
{
    return std::make_unique<InfoTool>();
}

}
