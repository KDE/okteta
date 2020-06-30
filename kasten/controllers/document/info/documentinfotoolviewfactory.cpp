/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentinfotoolviewfactory.hpp"

// lib
#include "documentinfotoolview.hpp"
#include "documentinfotool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

DocumentInfoToolViewFactory::DocumentInfoToolViewFactory() = default;

DocumentInfoToolViewFactory::~DocumentInfoToolViewFactory() = default;

QString DocumentInfoToolViewFactory::iconName() const { return QStringLiteral("documentinfo"); }
QString DocumentInfoToolViewFactory::title()    const { return i18nc("@title:window", "File Info"); }
QString DocumentInfoToolViewFactory::id()       const { return QStringLiteral("org.kde.okteta.DocumentInfoToolView"); }
SidePosition DocumentInfoToolViewFactory::defaultPosition() const { return LeftSidePosition; }

AbstractToolView* DocumentInfoToolViewFactory::create(AbstractTool* tool) const
{
    return new DocumentInfoToolView(qobject_cast<DocumentInfoTool*>(tool));
}

}
