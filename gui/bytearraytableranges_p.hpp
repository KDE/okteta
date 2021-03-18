/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BYTEARRAYTABLERANGES_P_HPP
#define OKTETA_BYTEARRAYTABLERANGES_P_HPP

// lib
#include "selection.hpp"

namespace Okteta {

class ByteArrayTableLayout;

class ByteArrayTableRangesPrivate
{
public:
    explicit ByteArrayTableRangesPrivate(ByteArrayTableLayout* layout)
        : layout(layout)
    {}

public:
    ByteArrayTableLayout* const layout;
    Selection previousSelection;
};

}

#endif

