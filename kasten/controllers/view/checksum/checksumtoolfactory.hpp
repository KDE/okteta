/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHECKSUMTOOLFACTORY_HPP
#define KASTEN_CHECKSUMTOOLFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT ChecksumToolFactory : public AbstractToolFactory
{
public:
    ChecksumToolFactory();
    ChecksumToolFactory(const ChecksumToolFactory&) = delete;
    ChecksumToolFactory(ChecksumToolFactory&&) = delete;

    ~ChecksumToolFactory() override;

    ChecksumToolFactory& operator=(const ChecksumToolFactory&) = delete;
    ChecksumToolFactory& operator=(ChecksumToolFactory&&) = delete;

public: // AbstractToolFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractTool> create() const override;
};

}

#endif
