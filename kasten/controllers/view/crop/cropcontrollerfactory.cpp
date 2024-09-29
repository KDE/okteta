/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "cropcontrollerfactory.hpp"

// lib
#include "cropcontroller.hpp"

namespace Kasten {

CropControllerFactory::CropControllerFactory() = default;

CropControllerFactory::~CropControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> CropControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<CropController>(guiClient);
}

}
