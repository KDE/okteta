/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentstoolviewfactory.hpp"

// lib
#include "documentstoolview.hpp"
#include "documentstool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

DocumentsToolViewFactory::DocumentsToolViewFactory() = default;

DocumentsToolViewFactory::~DocumentsToolViewFactory() = default;

QString DocumentsToolViewFactory::iconName() const { return QStringLiteral("view-history"); }
QString DocumentsToolViewFactory::title()    const { return i18nc("@title:window", "Documents"); }
QString DocumentsToolViewFactory::id()       const { return QStringLiteral("org.kde.kasten.DocumentsToolView"); }
SidePosition DocumentsToolViewFactory::defaultPosition() const { return LeftSidePosition; }

AbstractToolView* DocumentsToolViewFactory::create(AbstractTool* tool) const
{
    return new DocumentsToolView(qobject_cast<DocumentsTool*>(tool));
}

}
