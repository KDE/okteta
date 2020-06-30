/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacecontrollerfactory.hpp"

// lib
#include "replacecontroller.hpp"

namespace Kasten {

ReplaceControllerFactory::ReplaceControllerFactory(QWidget* parentWidget)
    : m_parentWidget(parentWidget)
{
}

ReplaceControllerFactory::~ReplaceControllerFactory() = default;

AbstractXmlGuiController* ReplaceControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new ReplaceController(guiClient, m_parentWidget);
}

}
