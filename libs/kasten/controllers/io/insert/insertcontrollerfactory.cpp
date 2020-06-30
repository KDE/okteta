/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "insertcontrollerfactory.hpp"

// lib
#include "insertcontroller.hpp"

namespace Kasten {

InsertControllerFactory::InsertControllerFactory(ModelCodecViewManager* modelCodecViewManager,
                                                 ModelCodecManager* modelCodecManager)
    : m_modelCodecViewManager(modelCodecViewManager)
    , m_modelCodecManager(modelCodecManager)
{
}

InsertControllerFactory::~InsertControllerFactory() = default;

AbstractXmlGuiController* InsertControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new InsertController(m_modelCodecViewManager, m_modelCodecManager, guiClient);
}

}
