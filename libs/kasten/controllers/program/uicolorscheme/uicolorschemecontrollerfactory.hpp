/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UICOLORSCHEMECONTROLLERFACTORY_HPP
#define KASTEN_UICOLORSCHEMECONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

class KXmlGuiWindow;

namespace Kasten {

class KASTENCONTROLLERS_EXPORT UiColorSchemeControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit UiColorSchemeControllerFactory(KXmlGuiWindow* window);
    UiColorSchemeControllerFactory(const UiColorSchemeControllerFactory&) = delete;
    UiColorSchemeControllerFactory(UiColorSchemeControllerFactory&&) = delete;

    ~UiColorSchemeControllerFactory() override;

    UiColorSchemeControllerFactory& operator=(const UiColorSchemeControllerFactory&) = delete;
    UiColorSchemeControllerFactory& operator=(UiColorSchemeControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    KXmlGuiWindow* const m_window;
};

}

#endif
