/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

class OKTETAKASTENCONTROLLERS_EXPORT SearchControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit SearchControllerFactory(QWidget* parentWidget);
    ~SearchControllerFactory() override;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;

private:
    QWidget* const m_parentWidget;
};

}

#endif
