/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CLIPBOARDCONTROLLERFACTORY_HPP
#define KASTEN_CLIPBOARDCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class KASTENCONTROLLERS_EXPORT ClipboardControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    ClipboardControllerFactory();
    ClipboardControllerFactory(const ClipboardControllerFactory&) = delete;

    ~ClipboardControllerFactory() override;

    ClipboardControllerFactory& operator=(const ClipboardControllerFactory&) = delete;

public: // AbstractXmlGuiControllerFactory API
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;
};

}

#endif
