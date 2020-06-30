/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "terminaltoolview.hpp"

// tool
#include "terminalview.hpp"
#include "terminaltool.hpp"

namespace Kasten {

TerminalToolView::TerminalToolView(TerminalTool* tool)
    : mWidget(new TerminalView(tool))
{
}

TerminalToolView::~TerminalToolView()
{
    delete mWidget;
}

QWidget* TerminalToolView::widget()    const { return mWidget; }
QString TerminalToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* TerminalToolView::tool() const { return mWidget->tool(); }

}
