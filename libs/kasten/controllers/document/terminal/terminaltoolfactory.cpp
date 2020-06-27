/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "terminaltoolfactory.hpp"

// lib
#include "terminaltool.hpp"

namespace Kasten {

TerminalToolFactory::TerminalToolFactory(DocumentSyncManager* documentSyncManager)
    : m_documentSyncManager(documentSyncManager)
{
}

TerminalToolFactory::~TerminalToolFactory() = default;

AbstractTool* TerminalToolFactory::create() const
{
    return new TerminalTool(m_documentSyncManager);
}

}
