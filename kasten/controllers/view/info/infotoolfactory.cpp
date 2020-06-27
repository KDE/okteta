/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "infotoolfactory.hpp"

// lib
#include "infotool.hpp"

namespace Kasten {

InfoToolFactory::InfoToolFactory() = default;

InfoToolFactory::~InfoToolFactory() = default;

AbstractTool* InfoToolFactory::create() const
{
    return new InfoTool();
}

}
