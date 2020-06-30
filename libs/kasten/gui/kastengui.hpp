/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_KASTENGUI_HPP
#define KASTEN_KASTENGUI_HPP

// Qt
#include <QFlags>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LOG_KASTEN_GUI)

namespace Kasten {

// keep in sync with both DockWidgetAre a and ToolBarArea
enum SidePosition
{
    LeftSidePosition = 0x1,
    RightSidePosition = 0x2,
    TopSidePosition = 0x4,
    BottomSidePosition = 0x8,

    SidePositionMask = 0xF,
    AllSidePositions = SidePositionMask,
    NoSidePosition = 0
};
enum SidePositionSizes
{
    SidePositionCount = 4
};

Q_DECLARE_FLAGS(SidePositions, SidePosition)

}

#endif
