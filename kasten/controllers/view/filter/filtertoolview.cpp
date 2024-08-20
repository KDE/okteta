/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "filtertoolview.hpp"

// tool
#include "filterview.hpp"
#include "filtertool.hpp"

namespace Kasten {

FilterToolView::FilterToolView(FilterTool* tool)
    : mWidget(std::make_unique<FilterView>(tool))
{
}

FilterToolView::~FilterToolView() = default;

QWidget* FilterToolView::widget()    const { return mWidget.get(); }
QString FilterToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* FilterToolView::tool() const { return mWidget->tool(); }

}

#include "moc_filtertoolview.cpp"
