/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "setremotecontrollerfactory.hpp"

// lib
#include "setremotecontroller.hpp"

namespace Kasten {

SetRemoteControllerFactory::SetRemoteControllerFactory(DocumentSyncManager* syncManager)
    : m_syncManager(syncManager)
{
}

SetRemoteControllerFactory::~SetRemoteControllerFactory() = default;

AbstractXmlGuiController* SetRemoteControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new SetRemoteController(m_syncManager, guiClient);
}

}
