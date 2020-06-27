/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "copyascontrollerfactory.hpp"

// lib
#include "copyascontroller.hpp"

namespace Kasten {

CopyAsControllerFactory::CopyAsControllerFactory(ModelCodecViewManager* modelCodecViewManager,
                                                 ModelCodecManager* modelCodecManager)
    : m_modelCodecViewManager(modelCodecViewManager)
    , m_modelCodecManager(modelCodecManager)
{
}

CopyAsControllerFactory::~CopyAsControllerFactory() = default;

AbstractXmlGuiController* CopyAsControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new CopyAsController(m_modelCodecViewManager, m_modelCodecManager, guiClient);
}

}
