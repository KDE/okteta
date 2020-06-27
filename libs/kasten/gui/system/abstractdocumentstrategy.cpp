/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractdocumentstrategy.hpp"
#include "abstractdocumentstrategy_p.hpp"

namespace Kasten {

AbstractDocumentStrategy::AbstractDocumentStrategy()
    : d_ptr(new AbstractDocumentStrategyPrivate(this))
{
}

AbstractDocumentStrategy::AbstractDocumentStrategy(AbstractDocumentStrategyPrivate* d)
    : d_ptr(d)
{
}

AbstractDocumentStrategy::~AbstractDocumentStrategy() = default;

}
