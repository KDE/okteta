/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "checksumtoolfactory.hpp"

// lib
#include "checksumtool.hpp"

namespace Kasten {

ChecksumToolFactory::ChecksumToolFactory() = default;

ChecksumToolFactory::~ChecksumToolFactory() = default;

AbstractTool* ChecksumToolFactory::create() const
{
    return new ChecksumTool();
}

}
