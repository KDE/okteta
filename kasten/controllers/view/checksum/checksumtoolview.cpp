/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "checksumtoolview.hpp"

// tool
#include "checksumview.hpp"
#include "checksumtool.hpp"

namespace Kasten {

ChecksumToolView::ChecksumToolView(ChecksumTool* tool)
    : mWidget(new ChecksumView(tool))
{
}

ChecksumToolView::~ChecksumToolView()
{
    delete mWidget;
}

QWidget* ChecksumToolView::widget()    const { return mWidget; }
QString ChecksumToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* ChecksumToolView::tool() const { return mWidget->tool(); }

}
