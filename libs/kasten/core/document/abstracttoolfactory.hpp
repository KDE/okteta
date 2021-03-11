/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTTOOLFACTORY_HPP
#define KASTEN_ABSTRACTTOOLFACTORY_HPP

// lib
#include "kastencore_export.hpp"

namespace Kasten {

class AbstractTool;

class KASTENCORE_EXPORT AbstractToolFactory
{
public:
    virtual ~AbstractToolFactory();

public: // API to be implemented
    virtual AbstractTool* create() const = 0;
};

inline AbstractToolFactory::~AbstractToolFactory() = default;

}

#endif
