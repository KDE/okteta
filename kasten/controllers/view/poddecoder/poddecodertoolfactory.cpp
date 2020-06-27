/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "poddecodertoolfactory.hpp"

// lib
#include "poddecodertool.hpp"

namespace Kasten {

PodDecoderToolFactory::PodDecoderToolFactory() = default;

PodDecoderToolFactory::~PodDecoderToolFactory() = default;

AbstractTool* PodDecoderToolFactory::create() const
{
    return new PODDecoderTool();
}

}
