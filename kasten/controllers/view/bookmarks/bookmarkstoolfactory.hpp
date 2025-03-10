/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BOOKMARKSTOOLFACTORY_HPP
#define KASTEN_BOOKMARKSTOOLFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT BookmarksToolFactory : public AbstractToolFactory
{
public:
    BookmarksToolFactory();
    BookmarksToolFactory(const BookmarksToolFactory&) = delete;
    BookmarksToolFactory(BookmarksToolFactory&&) = delete;

    ~BookmarksToolFactory() override;

    BookmarksToolFactory& operator=(const BookmarksToolFactory&) = delete;
    BookmarksToolFactory& operator=(BookmarksToolFactory&&) = delete;

public: // AbstractToolFactory API
    [[nodiscard]]
    std::unique_ptr<AbstractTool> create() const override;
};

}

#endif
