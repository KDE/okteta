/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SETREMOTECONTROLLERFACTORY_HPP
#define KASTEN_SETREMOTECONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class DocumentSyncManager;

class KASTENCONTROLLERS_EXPORT SetRemoteControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit SetRemoteControllerFactory(DocumentSyncManager* syncManager);
    SetRemoteControllerFactory(const SetRemoteControllerFactory&) = delete;
    SetRemoteControllerFactory(SetRemoteControllerFactory&&) = delete;

    ~SetRemoteControllerFactory() override;

    SetRemoteControllerFactory& operator=(const SetRemoteControllerFactory&) = delete;
    SetRemoteControllerFactory& operator=(SetRemoteControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    DocumentSyncManager* const m_syncManager;
};

}

#endif
