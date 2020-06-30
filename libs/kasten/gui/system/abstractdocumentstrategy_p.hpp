/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTDOCUMENTSTRATEGY_P_HPP
#define KASTEN_ABSTRACTDOCUMENTSTRATEGY_P_HPP

#include "abstractdocumentstrategy.hpp"

namespace Kasten {

class AbstractDocumentStrategyPrivate
{
public:
    explicit AbstractDocumentStrategyPrivate(AbstractDocumentStrategy* parent);

    virtual ~AbstractDocumentStrategyPrivate();

protected:
    AbstractDocumentStrategy* const q_ptr;
private:
    Q_DECLARE_PUBLIC(AbstractDocumentStrategy)
};

inline AbstractDocumentStrategyPrivate::AbstractDocumentStrategyPrivate(AbstractDocumentStrategy* parent)
    : q_ptr(parent)
{}

inline AbstractDocumentStrategyPrivate::~AbstractDocumentStrategyPrivate() = default;

}

#endif
