/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SEARCHCONTROLLERFACTORY_HPP
#define KASTEN_SEARCHCONTROLLERFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

class QWidget;

namespace Kasten {

class AbstractUserMessagesHandler;

class OKTETAKASTENCONTROLLERS_EXPORT SearchControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit SearchControllerFactory(AbstractUserMessagesHandler* userMessagesHandler, QWidget* parentWidget);
    SearchControllerFactory(const SearchControllerFactory&) = delete;
    SearchControllerFactory(SearchControllerFactory&&) = delete;

    ~SearchControllerFactory() override;

    SearchControllerFactory& operator=(const SearchControllerFactory&) = delete;
    SearchControllerFactory& operator=(SearchControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    AbstractUserMessagesHandler* const m_userMessagesHandler;
    QWidget* const m_parentWidget;
};

}

#endif
