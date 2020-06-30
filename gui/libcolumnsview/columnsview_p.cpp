/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "columnsview_p.hpp"

// Qt
#include <QScrollBar>

namespace Okteta {

static constexpr int DefaultSingleStep = 20;

void ColumnsViewPrivate::init()
{
    Q_Q(ColumnsView);

    auto viewport = q->viewport();
    viewport->setAttribute(Qt::WA_StaticContents);
    viewport->setBackgroundRole(QPalette::Base);
    q->horizontalScrollBar()->setSingleStep(DefaultSingleStep);
    q->verticalScrollBar()->setSingleStep(DefaultSingleStep);

    viewport->setFocusProxy(q);
    viewport->setFocusPolicy(Qt::WheelFocus);
}

}
