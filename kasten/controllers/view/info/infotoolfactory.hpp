/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_INFOTOOLFACTORY_HPP
#define KASTEN_INFOTOOLFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT InfoToolFactory : public AbstractToolFactory
{
public:
    InfoToolFactory();
    InfoToolFactory(const InfoToolFactory&) = delete;
    InfoToolFactory(InfoToolFactory&&) = delete;

    ~InfoToolFactory() override;

    InfoToolFactory& operator=(const InfoToolFactory&) = delete;
    InfoToolFactory& operator=(InfoToolFactory&&) = delete;

public: // AbstractToolFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractTool> create() const override;
};

}

#endif
