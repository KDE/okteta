/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FULLSCREENCONTROLLERFACTORY_HPP
#define KASTEN_FULLSCREENCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

class KXmlGuiWindow;

namespace Kasten {

class KASTENCONTROLLERS_EXPORT FullScreenControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit FullScreenControllerFactory(KXmlGuiWindow* window);
    ~FullScreenControllerFactory() override;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;

private:
    KXmlGuiWindow* const m_window;
};

}

#endif
