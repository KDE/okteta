/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWSTATUSCONTROLLERFACTORY_HPP
#define KASTEN_VIEWSTATUSCONTROLLERFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class StatusBar;

class OKTETAKASTENCONTROLLERS_EXPORT ViewStatusControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit ViewStatusControllerFactory(StatusBar* statusBar);
    ViewStatusControllerFactory(const ViewStatusControllerFactory&) = delete;
    ViewStatusControllerFactory(ViewStatusControllerFactory&&) = delete;

    ~ViewStatusControllerFactory() override;

    ViewStatusControllerFactory& operator=(const ViewStatusControllerFactory&) = delete;
    ViewStatusControllerFactory& operator=(ViewStatusControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    StatusBar* const m_statusBar;
};

}

#endif
