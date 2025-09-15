/*
    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charsetconversiontoolview.hpp"

// tool
#include "charsetconversionview.hpp"
#include "charsetconversiontool.hpp"

namespace Kasten {

CharsetConversionToolView::CharsetConversionToolView(CharsetConversionTool* tool,
                                                     AbstractUserMessagesHandler* userMessagesHandler)
    : mWidget(std::make_unique<CharsetConversionView>(tool, userMessagesHandler))
{
}

CharsetConversionToolView::~CharsetConversionToolView() = default;

QWidget* CharsetConversionToolView::widget()    const { return mWidget.get(); }
QString CharsetConversionToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* CharsetConversionToolView::tool() const { return mWidget->tool(); }

}

#include "moc_charsetconversiontoolview.cpp"
