/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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
    ~GotoOffsetControllerFactory() override;

public: // AbstractXmlGuiControllerFactory API
    AbstractXmlGuiController* create(KXMLGUIClient* guiClient) const override;

private:
    If::ToolInlineViewable* const m_toolInlineViewable;
};

}

#endif
