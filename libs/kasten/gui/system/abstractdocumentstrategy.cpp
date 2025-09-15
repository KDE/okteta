/*
    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractdocumentstrategy.hpp"
#include "abstractdocumentstrategy_p.hpp"

namespace Kasten {

AbstractDocumentStrategy::AbstractDocumentStrategy()
    : d_ptr(std::make_unique<AbstractDocumentStrategyPrivate>(this))
{
}

AbstractDocumentStrategy::AbstractDocumentStrategy(std::unique_ptr<AbstractDocumentStrategyPrivate>&& dd)
    : d_ptr(std::move(dd))
{
}

AbstractDocumentStrategy::~AbstractDocumentStrategy() = default;

}

#include "moc_abstractdocumentstrategy.cpp"
