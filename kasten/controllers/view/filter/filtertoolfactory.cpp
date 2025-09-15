/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filtertoolfactory.hpp"

// lib
#include "filtertool.hpp"

namespace Kasten {

FilterToolFactory::FilterToolFactory(AbstractUserMessagesHandler* userMessagesHandler)
    : m_userMessagesHandler(userMessagesHandler)
{
}

FilterToolFactory::~FilterToolFactory() = default;

std::unique_ptr<AbstractTool> FilterToolFactory::create() const
{
    return std::make_unique<FilterTool>(m_userMessagesHandler);
}

}
