/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_REPLACECONTROLLERFACTORY_HPP
#define KASTEN_REPLACECONTROLLERFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

class QWidget;

namespace Kasten {

class AbstractUserMessagesHandler;

class OKTETAKASTENCONTROLLERS_EXPORT ReplaceControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit ReplaceControllerFactory(AbstractUserMessagesHandler* userMessagesHandler, QWidget* parentWidget);
    ReplaceControllerFactory(const ReplaceControllerFactory&) = delete;
    ReplaceControllerFactory(ReplaceControllerFactory&&) = delete;

    ~ReplaceControllerFactory() override;

    ReplaceControllerFactory& operator=(const ReplaceControllerFactory&) = delete;
    ReplaceControllerFactory& operator=(ReplaceControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    AbstractUserMessagesHandler* const m_userMessagesHandler;
    QWidget* const m_parentWidget;
};

}

#endif
