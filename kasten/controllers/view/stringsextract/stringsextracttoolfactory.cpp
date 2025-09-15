/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stringsextracttoolfactory.hpp"

// lib
#include "stringsextracttool.hpp"

namespace Kasten {

StringsExtractToolFactory::StringsExtractToolFactory() = default;

StringsExtractToolFactory::~StringsExtractToolFactory() = default;

std::unique_ptr<AbstractTool> StringsExtractToolFactory::create() const
{
    return std::make_unique<StringsExtractTool>();
}

}
