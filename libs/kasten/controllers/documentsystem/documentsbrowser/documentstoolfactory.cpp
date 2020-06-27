/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentstoolfactory.hpp"

// lib
#include "documentstool.hpp"

namespace Kasten {

DocumentsToolFactory::DocumentsToolFactory(DocumentManager* documentManager)
    : m_documentManager(documentManager)
{
}

DocumentsToolFactory::~DocumentsToolFactory() = default;

AbstractTool* DocumentsToolFactory::create() const
{
    return new DocumentsTool(m_documentManager);
}

}
