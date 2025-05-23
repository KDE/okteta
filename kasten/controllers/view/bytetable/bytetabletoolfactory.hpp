/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTETABLETOOLFACTORY_HPP
#define KASTEN_BYTETABLETOOLFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class AbstractUserMessagesHandler;

class OKTETAKASTENCONTROLLERS_EXPORT ByteTableToolFactory : public AbstractToolFactory
{
public:
    explicit ByteTableToolFactory(AbstractUserMessagesHandler* userMessagesHandler);
    ByteTableToolFactory(const ByteTableToolFactory&) = delete;
    ByteTableToolFactory(ByteTableToolFactory&&) = delete;

    ~ByteTableToolFactory() override;

    ByteTableToolFactory& operator=(const ByteTableToolFactory&) = delete;
    ByteTableToolFactory& operator=(ByteTableToolFactory&&) = delete;

public: // AbstractToolFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractTool> create() const override;

private:
    AbstractUserMessagesHandler* const m_userMessagesHandler;
};

}

#endif
