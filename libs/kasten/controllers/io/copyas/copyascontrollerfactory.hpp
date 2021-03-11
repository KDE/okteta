/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_COPYASCONTROLLERFACTORY_HPP
#define KASTEN_COPYASCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class ModelCodecViewManager;
class ModelCodecManager;

class KASTENCONTROLLERS_EXPORT CopyAsControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit CopyAsControllerFactory(ModelCodecViewManager* modelCodecViewManager,
                                     ModelCodecManager* modelCodecManager);
    ~CopyAsControllerFactory() override;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;

private:
    ModelCodecViewManager* const m_modelCodecViewManager;
    ModelCodecManager* const m_modelCodecManager;
};

}

#endif
