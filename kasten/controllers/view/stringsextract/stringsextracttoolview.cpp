/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stringsextracttoolview.hpp"

// tool
#include "stringsextractview.hpp"
#include "stringsextracttool.hpp"

namespace Kasten {

StringsExtractToolView::StringsExtractToolView(StringsExtractTool* tool,
                                               AbstractUserMessagesHandler* userMessagseHandler)
    : mWidget(std::make_unique<StringsExtractView>(tool, userMessagseHandler))
{
}

StringsExtractToolView::~StringsExtractToolView() = default;

QWidget* StringsExtractToolView::widget()    const { return mWidget.get(); }
QString StringsExtractToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* StringsExtractToolView::tool() const { return mWidget->tool(); }

}

#include "moc_stringsextracttoolview.cpp"
