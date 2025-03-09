/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CROPCONTROLLERFACTORY_HPP
#define KASTEN_CROPCONTROLLERFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT CropControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    CropControllerFactory();
    CropControllerFactory(const CropControllerFactory&) = delete;
    CropControllerFactory(CropControllerFactory&&) = delete;

    ~CropControllerFactory() override;

    CropControllerFactory& operator=(const CropControllerFactory&) = delete;
    CropControllerFactory& operator=(CropControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;
};

}

#endif
