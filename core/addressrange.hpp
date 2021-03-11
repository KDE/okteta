/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ADDRESSRANGE_HPP
#define OKTETA_ADDRESSRANGE_HPP

// lib
#include "numberrange.hpp"
#include "address.hpp"
#include "size.hpp"

namespace Okteta {

using AddressRange = NumberRange<Address, Size>;

}

#endif
