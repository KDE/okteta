/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "infotoolview.hpp"

// tool
#include "infoview.hpp"
#include "infotool.hpp"

namespace Kasten {

InfoToolView::InfoToolView(InfoTool* tool)
    : mWidget(std::make_unique<InfoView>(tool))
{
}

InfoToolView::~InfoToolView() = default;

QWidget* InfoToolView::widget()    const { return mWidget.get(); }
QString InfoToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* InfoToolView::tool() const { return mWidget->tool(); }

}

#include "moc_infotoolview.cpp"
