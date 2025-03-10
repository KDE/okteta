/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODIFIEDBARCONTROLLERFACTORY_HPP
#define KASTEN_MODIFIEDBARCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class StatusBar;

class KASTENCONTROLLERS_EXPORT ModifiedBarControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit ModifiedBarControllerFactory(StatusBar* statusBar);
    ModifiedBarControllerFactory(const ModifiedBarControllerFactory&) = delete;
    ModifiedBarControllerFactory(ModifiedBarControllerFactory&&) = delete;

    ~ModifiedBarControllerFactory() override;

    ModifiedBarControllerFactory& operator=(const ModifiedBarControllerFactory&) = delete;
    ModifiedBarControllerFactory& operator=(ModifiedBarControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    StatusBar* const m_statusBar;
};

}

#endif
