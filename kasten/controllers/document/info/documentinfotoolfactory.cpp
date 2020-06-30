/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentinfotoolfactory.hpp"

// lib
#include "documentinfotool.hpp"

namespace Kasten {

DocumentInfoToolFactory::DocumentInfoToolFactory(DocumentSyncManager* syncManager)
    : m_syncManager(syncManager)
{
}

DocumentInfoToolFactory::~DocumentInfoToolFactory() = default;

AbstractTool* DocumentInfoToolFactory::create() const
{
    return new DocumentInfoTool(m_syncManager);
}

}
