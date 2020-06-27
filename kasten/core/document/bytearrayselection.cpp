/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayselection.hpp"

namespace Kasten {

ByteArraySelection::ByteArraySelection() = default;

ByteArraySelection::~ByteArraySelection() = default;

bool ByteArraySelection::isValid() const { return mRange.isValid(); }
Okteta::AddressRange ByteArraySelection::range() const { return mRange; }

void ByteArraySelection::setRange(const Okteta::AddressRange& range) { mRange = range; }

}
