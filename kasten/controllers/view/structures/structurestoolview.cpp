/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structurestoolview.hpp"
// tool
#include "structureview.hpp"
#include "structurestool.hpp"

namespace Kasten {

StructuresToolView::StructuresToolView(StructuresTool* tool) :
    mWidget(new StructureView(tool))
{
}

StructuresToolView::~StructuresToolView()
{
    delete mWidget;
}

QWidget* StructuresToolView::widget() const
{
    return mWidget;
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
