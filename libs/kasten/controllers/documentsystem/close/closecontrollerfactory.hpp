/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CLOSECONTROLLERFACTORY_HPP
#define KASTEN_CLOSECONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class AbstractDocumentStrategy;

class KASTENCONTROLLERS_EXPORT CloseControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit CloseControllerFactory(AbstractDocumentStrategy* documentStrategy);
    ~CloseControllerFactory() override;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;

private:
    AbstractDocumentStrategy* const m_documentStrategy;
};

}

#endif
