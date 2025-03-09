/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ZOOMBARCONTROLLERFACTORY_HPP
#define KASTEN_ZOOMBARCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class StatusBar;

class KASTENCONTROLLERS_EXPORT ZoomBarControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit ZoomBarControllerFactory(StatusBar* statusBar);
    ZoomBarControllerFactory(const ZoomBarControllerFactory&) = delete;
    ZoomBarControllerFactory(ZoomBarControllerFactory&&) = delete;

    ~ZoomBarControllerFactory() override;

    ZoomBarControllerFactory& operator=(const ZoomBarControllerFactory&) = delete;
    ZoomBarControllerFactory& operator=(ZoomBarControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    StatusBar* const m_statusBar;
};

}

#endif
