/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWPROFILESMANAGECONTROLLERFACTORY_HPP
#define KASTEN_VIEWPROFILESMANAGECONTROLLERFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

class QWidget;

namespace Kasten {

class ByteArrayViewProfileManager;

class OKTETAKASTENCONTROLLERS_EXPORT ViewProfilesManageControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    ViewProfilesManageControllerFactory(ByteArrayViewProfileManager* viewProfileManager,
                                        QWidget* parentWidget);
    ~ViewProfilesManageControllerFactory() override;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;

private:
    ByteArrayViewProfileManager* const m_viewProfileManager;
    QWidget* const m_parentWidget;
};

}

#endif
