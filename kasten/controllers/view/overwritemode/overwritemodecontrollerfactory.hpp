/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_OVERWRITEMODECONTROLLERFACTORY_HPP
#define KASTEN_OVERWRITEMODECONTROLLERFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT OverwriteModeControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    OverwriteModeControllerFactory();
    OverwriteModeControllerFactory(const OverwriteModeControllerFactory&) = delete;
    OverwriteModeControllerFactory(OverwriteModeControllerFactory&&) = delete;

    ~OverwriteModeControllerFactory() override;

    OverwriteModeControllerFactory& operator=(const OverwriteModeControllerFactory&) = delete;
    OverwriteModeControllerFactory& operator=(OverwriteModeControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;
};

}

#endif
