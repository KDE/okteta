/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "exportcontrollerfactory.hpp"

// lib
#include "exportcontroller.hpp"

namespace Kasten {

ExportControllerFactory::ExportControllerFactory(ModelCodecViewManager* modelCodecViewManager,
                                                 ModelCodecManager* modelCodecManager)
    : m_modelCodecViewManager(modelCodecViewManager)
    , m_modelCodecManager(modelCodecManager)
{
}

ExportControllerFactory::~ExportControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> ExportControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<ExportController>(m_modelCodecViewManager, m_modelCodecManager, guiClient);
}

}
