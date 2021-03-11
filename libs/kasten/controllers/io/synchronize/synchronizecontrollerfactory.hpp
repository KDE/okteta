/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SYNCHRONIZECONTROLLERFACTORY_HPP
#define KASTEN_SYNCHRONIZECONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class DocumentSyncManager;

class KASTENCONTROLLERS_EXPORT SynchronizeControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit SynchronizeControllerFactory(DocumentSyncManager* syncManager);
    ~SynchronizeControllerFactory() override;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;

private:
    DocumentSyncManager* const m_syncManager;
};

}

#endif
