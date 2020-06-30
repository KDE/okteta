/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "gotooffsettoolview.hpp"

// tool
#include "gotooffsetview.hpp"
#include "gotooffsettool.hpp"

namespace Kasten {

GotoOffsetToolView::GotoOffsetToolView(GotoOffsetTool* tool)
    : mWidget(new GotoOffsetView(tool))
{
}

GotoOffsetToolView::~GotoOffsetToolView()
{
    delete mWidget;
}

QWidget* GotoOffsetToolView::widget()    const { return mWidget; }
QString GotoOffsetToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* GotoOffsetToolView::tool() const { return mWidget->tool(); }

}
