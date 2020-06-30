/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filesystembrowsertoolview.hpp"

// tool
#include "filesystembrowserview.hpp"
#include "filesystembrowsertool.hpp"

namespace Kasten {

FileSystemBrowserToolView::FileSystemBrowserToolView(FileSystemBrowserTool* tool)
    : mWidget(new FileSystemBrowserView(tool))
{
}

FileSystemBrowserToolView::~FileSystemBrowserToolView() = default;

QWidget* FileSystemBrowserToolView::widget()    const { return mWidget; }
QString FileSystemBrowserToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* FileSystemBrowserToolView::tool() const { return mWidget->tool(); }

}
