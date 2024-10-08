/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytetabletoolview.hpp"

// tool
#include "bytetableview.hpp"
#include "bytetabletool.hpp"

namespace Kasten {

ByteTableToolView::ByteTableToolView(ByteTableTool* tool)
    : mWidget(std::make_unique<ByteTableView>(tool))
{
}

ByteTableToolView::~ByteTableToolView() = default;

QWidget* ByteTableToolView::widget()    const { return mWidget.get(); }
QString ByteTableToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* ByteTableToolView::tool() const { return mWidget->tool(); }

}

#include "moc_bytetabletoolview.cpp"
