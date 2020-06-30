/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractpiecetablechange.hpp"

// Qt
#include <Qt>

namespace KPieceTable {

bool AbstractPieceTableChange::merge(const AbstractPieceTableChange* other)
{
    Q_UNUSED(other)
    return false;
}

Address AbstractPieceTableChange::storageOffset() const { return -1; }

Size AbstractPieceTableChange::dataSize() const { return 0; }

}
