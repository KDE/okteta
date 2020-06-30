/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentinfotoolview.hpp"

// tool
#include "documentinfoview.hpp"
#include "documentinfotool.hpp"

namespace Kasten {

DocumentInfoToolView::DocumentInfoToolView(DocumentInfoTool* tool)
    : mWidget(new DocumentInfoView(tool))
{
}

DocumentInfoToolView::~DocumentInfoToolView()
{
    delete mWidget;
}

QWidget* DocumentInfoToolView::widget()    const { return mWidget; }
QString DocumentInfoToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* DocumentInfoToolView::tool() const { return mWidget->tool(); }

}
