/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "poddecodertoolview.hpp"

// tool
#include "podtableview.hpp"
#include "poddecodertool.hpp"

namespace Kasten {

PODDecoderToolView::PODDecoderToolView(PODDecoderTool* tool)
    : mWidget(new PODTableView(tool))
{
}

PODDecoderToolView::~PODDecoderToolView()
{
    delete mWidget;
}

QWidget* PODDecoderToolView::widget()    const { return mWidget; }
QString PODDecoderToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* PODDecoderToolView::tool() const { return mWidget->tool(); }

}
