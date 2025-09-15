/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentstoolview.hpp"

// tool
#include "documentsview.hpp"
#include "documentstool.hpp"

namespace Kasten {

DocumentsToolView::DocumentsToolView(DocumentsTool* tool)
    : mWidget(std::make_unique<DocumentsView>(tool))
{
}

DocumentsToolView::~DocumentsToolView() = default;

QWidget* DocumentsToolView::widget()    const { return mWidget.get(); }
QString DocumentsToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* DocumentsToolView::tool() const { return mWidget->tool(); }

}

#include "moc_documentstoolview.cpp"
