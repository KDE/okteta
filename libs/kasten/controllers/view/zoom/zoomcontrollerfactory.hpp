/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ZOOMCONTROLLERFACTORY_HPP
#define KASTEN_ZOOMCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class KASTENCONTROLLERS_EXPORT ZoomControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    ZoomControllerFactory();
    ZoomControllerFactory(const ZoomControllerFactory&) = delete;
    ZoomControllerFactory(ZoomControllerFactory&&) = delete;

    ~ZoomControllerFactory() override;

    ZoomControllerFactory& operator=(const ZoomControllerFactory&) = delete;
    ZoomControllerFactory& operator=(ZoomControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;
};

}

#endif
