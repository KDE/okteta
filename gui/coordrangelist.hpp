/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_COORDRANGELIST_HPP
#define OKTETA_COORDRANGELIST_HPP

// lib
#include "coordrange.hpp"
// Qt
#include <QLinkedList>

namespace Okteta {

/**
   @author Friedrich W. H.  Kossebau
 */
class CoordRangeList : public QLinkedList<CoordRange>
{
public:
    CoordRangeList();

    ~CoordRangeList();

public:
    void addCoordRange(const CoordRange& coordRange);
};

}

#endif
