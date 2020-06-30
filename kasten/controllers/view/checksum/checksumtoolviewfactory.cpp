/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "checksumtoolviewfactory.hpp"

// lib
#include "checksumtoolview.hpp"
#include "checksumtool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

ChecksumToolViewFactory::ChecksumToolViewFactory() = default;

ChecksumToolViewFactory::~ChecksumToolViewFactory() = default;

QString ChecksumToolViewFactory::iconName() const { return QStringLiteral("accessories-calculator"); }
QString ChecksumToolViewFactory::title()       const { return i18nc("@title:window of the tool to calculate checksums", "Checksum"); }
QString ChecksumToolViewFactory:: id()      const { return QStringLiteral("org.kde.okteta.ChecksumToolView"); }
SidePosition ChecksumToolViewFactory::defaultPosition() const { return BottomSidePosition; }

AbstractToolView* ChecksumToolViewFactory::create(AbstractTool* tool) const
{
    return new ChecksumToolView(qobject_cast<ChecksumTool*>(tool));
}

}
