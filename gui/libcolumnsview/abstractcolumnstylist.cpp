/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractcolumnstylist.hpp"
#include "abstractcolumnstylist_p.hpp"

namespace Okteta {

AbstractColumnStylist::AbstractColumnStylist()
    : d_ptr(new AbstractColumnStylistPrivate())
{
}

AbstractColumnStylist::AbstractColumnStylist(AbstractColumnStylistPrivate* d)
    : d_ptr(d)
{
}

AbstractColumnStylist::~AbstractColumnStylist() = default;

}
