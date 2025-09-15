/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "poddecodertoolfactory.hpp"

// lib
#include "poddecodertool.hpp"

namespace Kasten {

PodDecoderToolFactory::PodDecoderToolFactory(AbstractUserMessagesHandler* userMessagesHandler)
    : m_userMessagesHandler(userMessagesHandler)
{
}

PodDecoderToolFactory::~PodDecoderToolFactory() = default;

std::unique_ptr<AbstractTool> PodDecoderToolFactory::create() const
{
    return std::make_unique<PODDecoderTool>(m_userMessagesHandler);
}

}
