/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_GOTOOFFSETCONTROLLERFACTORY_HPP
#define KASTEN_GOTOOFFSETCONTROLLERFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

namespace If {
class ToolInlineViewable;
}

class OKTETAKASTENCONTROLLERS_EXPORT GotoOffsetControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit GotoOffsetControllerFactory(If::ToolInlineViewable* toolInlineViewable);
    GotoOffsetControllerFactory(const GotoOffsetControllerFactory&) = delete;
    GotoOffsetControllerFactory(GotoOffsetControllerFactory&&) = delete;

    ~GotoOffsetControllerFactory() override;

    GotoOffsetControllerFactory& operator=(const GotoOffsetControllerFactory&) = delete;
    GotoOffsetControllerFactory& operator=(GotoOffsetControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    If::ToolInlineViewable* const m_toolInlineViewable;
};

}

#endif
