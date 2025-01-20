/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGSEXTRACTTOOLVIEWFACTORY_HPP
#define KASTEN_STRINGSEXTRACTTOOLVIEWFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten gui
#include <Kasten/AbstractToolViewFactory>

namespace Kasten {

class AbstractUserMessagesHandler;

class OKTETAKASTENCONTROLLERS_EXPORT StringsExtractToolViewFactory : public AbstractToolViewFactory
{
public:
    StringsExtractToolViewFactory(AbstractUserMessagesHandler* userMessagesHandler);
    StringsExtractToolViewFactory(const StringsExtractToolViewFactory&) = delete;

    ~StringsExtractToolViewFactory() override;

    StringsExtractToolViewFactory& operator=(const StringsExtractToolViewFactory&) = delete;

public: // AbstractToolViewFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractToolView> create(AbstractTool* tool) const override;

    [[nodiscard]]
    QString iconName() const override;
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    QString id() const override;
    [[nodiscard]]
    SidePosition defaultPosition() const override;

private:
    AbstractUserMessagesHandler* const m_userMessagesHandler;
};

}

#endif
