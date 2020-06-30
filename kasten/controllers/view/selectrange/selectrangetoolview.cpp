/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "selectrangetoolview.hpp"

// tool
#include "selectrangeview.hpp"
#include "selectrangetool.hpp"

namespace Kasten {

SelectRangeToolView::SelectRangeToolView(SelectRangeTool* tool)
    : mWidget(new SelectRangeView(tool))
{
}

SelectRangeToolView::~SelectRangeToolView()
{
    delete mWidget;
}

QWidget* SelectRangeToolView::widget()    const { return mWidget; }
QString SelectRangeToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* SelectRangeToolView::tool() const { return mWidget->tool(); }

}
