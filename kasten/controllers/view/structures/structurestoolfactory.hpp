/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTURESTOOLFACTORY_HPP
#define KASTEN_STRUCTURESTOOLFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT StructuresToolFactory : public AbstractToolFactory
{
public:
    StructuresToolFactory();
    StructuresToolFactory(const StructuresToolFactory&) = delete;
    StructuresToolFactory(StructuresToolFactory&&) = delete;

    ~StructuresToolFactory() override;

    StructuresToolFactory& operator=(const StructuresToolFactory&) = delete;
    StructuresToolFactory& operator=(StructuresToolFactory&&) = delete;

public: // AbstractToolFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractTool> create() const override;
};

}

#endif
