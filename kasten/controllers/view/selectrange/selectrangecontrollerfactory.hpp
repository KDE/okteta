/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SELECTRANGECONTROLLERFACTORY_HPP
#define KASTEN_SELECTRANGECONTROLLERFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractXmlGuiControllerFactory>

namespace Kasten {

namespace If {
class ToolInlineViewable;
}

class OKTETAKASTENCONTROLLERS_EXPORT SelectRangeControllerFactory : public AbstractXmlGuiControllerFactory
{
public:
    explicit SelectRangeControllerFactory(If::ToolInlineViewable* toolInlineViewable);
    SelectRangeControllerFactory(const SelectRangeControllerFactory&) = delete;
    SelectRangeControllerFactory(SelectRangeControllerFactory&&) = delete;

    ~SelectRangeControllerFactory() override;

    SelectRangeControllerFactory& operator=(const SelectRangeControllerFactory&) = delete;
    SelectRangeControllerFactory& operator=(SelectRangeControllerFactory&&) = delete;

public: // AbstractXmlGuiControllerFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractXmlGuiController> create(KXMLGUIClient* guiClient) const override;

private:
    If::ToolInlineViewable* const m_toolInlineViewable;
};

}

#endif
