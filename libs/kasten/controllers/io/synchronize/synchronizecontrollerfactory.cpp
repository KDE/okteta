/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "synchronizecontrollerfactory.hpp"

// lib
#include "synchronizecontroller.hpp"

namespace Kasten {

SynchronizeControllerFactory::SynchronizeControllerFactory(DocumentSyncManager* syncManager)
    : m_syncManager(syncManager)
{
}

SynchronizeControllerFactory::~SynchronizeControllerFactory() = default;

AbstractXmlGuiController* SynchronizeControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new SynchronizeController(m_syncManager, guiClient);
}

}
