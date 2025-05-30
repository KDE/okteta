/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SWITCHVIEWCONTROLLERFACTORY_HPP
#define KASTEN_SWITCHVIEWCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class AbstractGroupedViews;

class KASTENCONTROLLERS_EXPORT SwitchViewControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit SwitchViewControllerFactory(AbstractGroupedViews* groupedViews);
    SwitchViewControllerFactory(const SwitchViewControllerFactory&) = delete;
    SwitchViewControllerFactory(SwitchViewControllerFactory&&) = delete;

    ~SwitchViewControllerFactory() override;

    SwitchViewControllerFactory& operator=(const SwitchViewControllerFactory&) = delete;
    SwitchViewControllerFactory& operator=(SwitchViewControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    AbstractGroupedViews* const m_groupedViews;
};

}

#endif
