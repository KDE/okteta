/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stringsextracttoolview.hpp"

// tool
#include "stringsextractview.hpp"
#include "stringsextracttool.hpp"

namespace Kasten {

StringsExtractToolView::StringsExtractToolView(StringsExtractTool* tool)
    : mWidget(new StringsExtractView(tool))
{
}

StringsExtractToolView::~StringsExtractToolView()
{
    delete mWidget;
}

QWidget* StringsExtractToolView::widget()    const { return mWidget; }
QString StringsExtractToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* StringsExtractToolView::tool() const { return mWidget->tool(); }

}
