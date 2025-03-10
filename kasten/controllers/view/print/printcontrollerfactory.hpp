/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRINTCONTROLLERFACTORY_HPP
#define KASTEN_PRINTCONTROLLERFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class AbstractUserMessagesHandler;

class OKTETAKASTENCONTROLLERS_EXPORT PrintControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    PrintControllerFactory(AbstractUserMessagesHandler* userMessagesHandler);
    PrintControllerFactory(const PrintControllerFactory&) = delete;
    PrintControllerFactory(PrintControllerFactory&&) = delete;

    ~PrintControllerFactory() override;

    PrintControllerFactory& operator=(const PrintControllerFactory&) = delete;
    PrintControllerFactory& operator=(PrintControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    AbstractUserMessagesHandler* const m_userMessagesHandler;
};

}

#endif
