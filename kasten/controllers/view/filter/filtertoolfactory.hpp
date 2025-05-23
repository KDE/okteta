/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FILTERTOOLFACTORY_HPP
#define KASTEN_FILTERTOOLFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class AbstractUserMessagesHandler;

class OKTETAKASTENCONTROLLERS_EXPORT FilterToolFactory : public AbstractToolFactory
{
public:
    explicit FilterToolFactory(AbstractUserMessagesHandler* userMessagesHandler);
    FilterToolFactory(const FilterToolFactory&) = delete;
    FilterToolFactory(FilterToolFactory&&) = delete;

    ~FilterToolFactory() override;

    FilterToolFactory& operator=(const FilterToolFactory&) = delete;
    FilterToolFactory& operator=(FilterToolFactory&&) = delete;

public: // AbstractToolFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractTool> create() const override;

private:
    AbstractUserMessagesHandler* const m_userMessagesHandler;
};

}

#endif
