/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VERSIONVIEWTOOLFACTORY_HPP
#define KASTEN_VERSIONVIEWTOOLFACTORY_HPP

// lib
#include "kastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class KASTENCONTROLLERS_EXPORT VersionViewToolFactory : public AbstractToolFactory
{
public:
    VersionViewToolFactory();
    ~VersionViewToolFactory() override;

public: // AbstractToolFactory API
    AbstractTool* create() const override;
};

}

#endif
