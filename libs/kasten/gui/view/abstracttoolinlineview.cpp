/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstracttoolinlineview.hpp"

namespace Kasten {

class AbstractToolInlineViewPrivate
{
public:
};


AbstractToolInlineView::AbstractToolInlineView() = default;

AbstractToolInlineView::~AbstractToolInlineView() = default;

void AbstractToolInlineView::activate()
{
}

}

#include "moc_abstracttoolinlineview.cpp"
