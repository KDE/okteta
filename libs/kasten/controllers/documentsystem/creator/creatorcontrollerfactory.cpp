/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "creatorcontrollerfactory.hpp"

// lib
#include "creatorcontroller.hpp"

namespace Kasten {

CreatorControllerFactory::CreatorControllerFactory(ModelCodecManager* modelCodecManager,
                                                   AbstractDocumentStrategy* documentStrategy)
    : m_modelCodecManager(modelCodecManager)
    , m_documentStrategy(documentStrategy)
{
}

CreatorControllerFactory::~CreatorControllerFactory() = default;

AbstractXmlGuiController* CreatorControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new CreatorController(m_modelCodecManager, m_documentStrategy, guiClient);
}

}
