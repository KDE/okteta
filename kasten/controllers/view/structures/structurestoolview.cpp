/*
    SPDX-FileCopyrightText: 2009 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structurestoolview.hpp"

// tool
#include "structureview.hpp"
#include "structurestool.hpp"

namespace Kasten {

StructuresToolView::StructuresToolView(StructuresTool* tool)
    : mWidget(std::make_unique<StructureView>(tool))
{
}

StructuresToolView::~StructuresToolView() = default;

QWidget* StructuresToolView::widget() const
{
    return mWidget.get();
}
QString StructuresToolView::title() const
{
    return mWidget->tool()->title();
}
AbstractTool* StructuresToolView::tool() const
{
    return mWidget->tool();
}

}

#include "moc_structurestoolview.cpp"
