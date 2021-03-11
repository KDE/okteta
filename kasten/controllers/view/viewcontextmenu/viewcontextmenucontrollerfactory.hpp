/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWCONTEXTMENUCONTROLLERFACTORY_HPP
#define KASTEN_VIEWCONTEXTMENUCONTROLLERFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT ViewContextMenuControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    ViewContextMenuControllerFactory();
    ~ViewContextMenuControllerFactory() override;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;
};

}

#endif
