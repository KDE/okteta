/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_EXPORTCONTROLLERFACTORY_HPP
#define KASTEN_EXPORTCONTROLLERFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

class ModelCodecViewManager;
class ModelCodecManager;

class KASTENCONTROLLERS_EXPORT ExportControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit ExportControllerFactory(ModelCodecViewManager* modelCodecViewManager,
                                     ModelCodecManager* modelCodecManager);
    ExportControllerFactory(const ExportControllerFactory&) = delete;
    ExportControllerFactory(ExportControllerFactory&&) = delete;

    ~ExportControllerFactory() override;

    ExportControllerFactory& operator=(const ExportControllerFactory&) = delete;
    ExportControllerFactory& operator=(ExportControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    ModelCodecViewManager* const m_modelCodecViewManager;
    ModelCodecManager* const m_modelCodecManager;
};

}

#endif
