/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filesystembrowsertoolviewfactory.hpp"

// lib
#include "filesystembrowsertoolview.hpp"
#include "filesystembrowsertool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

FileSystemBrowserToolViewFactory::FileSystemBrowserToolViewFactory() = default;

FileSystemBrowserToolViewFactory::~FileSystemBrowserToolViewFactory() = default;

QString FileSystemBrowserToolViewFactory::iconName() const { return QStringLiteral("folder"); }
QString FileSystemBrowserToolViewFactory::title()    const { return i18nc("@title:window", "Filesystem"); }
QString FileSystemBrowserToolViewFactory::id()       const { return QStringLiteral("org.kde.kasten.FileSystemBrowserToolView"); }
SidePosition FileSystemBrowserToolViewFactory::defaultPosition() const { return LeftSidePosition; }

AbstractToolView* FileSystemBrowserToolViewFactory::create(AbstractTool* tool) const
{
    return new FileSystemBrowserToolView(qobject_cast<FileSystemBrowserTool*>(tool));
}

}
