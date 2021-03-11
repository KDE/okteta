/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWLISTMENUCONTROLLERFACTORY_HPP
#define KASTEN_VIEWLISTMENUCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class ViewManager;
class AbstractGroupedViews;

class KASTENCONTROLLERS_EXPORT ViewListMenuControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit ViewListMenuControllerFactory(ViewManager* viewManager, AbstractGroupedViews* groupedViews);
    ~ViewListMenuControllerFactory() override;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;

private:
    ViewManager* const m_viewManager;
    AbstractGroupedViews* const m_groupedViews;
};

}

#endif
