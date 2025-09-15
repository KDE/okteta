/*
    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charsetconversiontoolfactory.hpp"

// lib
#include "charsetconversiontool.hpp"

namespace Kasten {

CharsetConversionToolFactory::CharsetConversionToolFactory() = default;

CharsetConversionToolFactory::~CharsetConversionToolFactory() = default;

std::unique_ptr<AbstractTool> CharsetConversionToolFactory::create() const
{
    return std::make_unique<CharsetConversionTool>();
}

}
