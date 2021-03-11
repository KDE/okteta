/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TOOLLISTMENUCONTROLLERFACTORY_HPP
#define KASTEN_TOOLLISTMENUCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

namespace If {
class WidgetsDockable;
}

class KASTENCONTROLLERS_EXPORT ToolListMenuControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit ToolListMenuControllerFactory(If::WidgetsDockable* widgetsDockable);
    ~ToolListMenuControllerFactory() override;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;

private:
    If::WidgetsDockable* const m_widgetsDockable;
};

}

#endif
