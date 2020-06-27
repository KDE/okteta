/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filesystembrowsertoolfactory.hpp"

// lib
#include "filesystembrowsertool.hpp"

namespace Kasten {

FileSystemBrowserToolFactory::FileSystemBrowserToolFactory(DocumentSyncManager* documentSyncManager)
    : m_documentSyncManager(documentSyncManager)
{
}

FileSystemBrowserToolFactory::~FileSystemBrowserToolFactory() = default;

AbstractTool* FileSystemBrowserToolFactory::create() const
{
    return new FileSystemBrowserTool(m_documentSyncManager);
}

}
