/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWAREACONTEXTMENUCONTROLLERFACTORY_HPP
#define KASTEN_VIEWAREACONTEXTMENUCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class MultiViewAreas;
class DocumentSyncManager;

class KASTENCONTROLLERS_EXPORT ViewAreaContextMenuControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    ViewAreaContextMenuControllerFactory(MultiViewAreas* multiViewAreas,
                                         DocumentSyncManager* syncManager);
    ViewAreaContextMenuControllerFactory(const ViewAreaContextMenuControllerFactory&) = delete;

    ~ViewAreaContextMenuControllerFactory() override;

    ViewAreaContextMenuControllerFactory& operator=(const ViewAreaContextMenuControllerFactory&) = delete;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;

private:
    MultiViewAreas* const m_multiViewAreas;
    DocumentSyncManager* const m_syncManager;
};

}

#endif
