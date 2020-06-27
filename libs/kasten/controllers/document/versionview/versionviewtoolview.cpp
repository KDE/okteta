/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "versionviewtoolview.hpp"

// tool
#include "versionview.hpp"
#include "versionviewtool.hpp"

namespace Kasten {

VersionViewToolView::VersionViewToolView(VersionViewTool* tool)
    : mWidget(new VersionView(tool))
{
}

VersionViewToolView::~VersionViewToolView() = default;

QWidget* VersionViewToolView::widget()    const { return mWidget; }
QString VersionViewToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* VersionViewToolView::tool() const { return mWidget->tool(); }

}
