/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractview.hpp"

namespace Kasten {

AbstractView::AbstractView(AbstractModel* baseModel)
    : AbstractModel(baseModel)
{
}

AbstractView::~AbstractView() = default;

}
