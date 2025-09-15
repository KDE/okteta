/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYFILTERFACTORY_HPP
#define KASTEN_BYTEARRAYFILTERFACTORY_HPP

// Std
#include <memory>
#include <vector>

class AbstractByteArrayFilter;

class ByteArrayFilterFactory
{
public:
    ByteArrayFilterFactory() = delete;
    ByteArrayFilterFactory(const ByteArrayFilterFactory&) = delete;
    ByteArrayFilterFactory(ByteArrayFilterFactory&&) = delete;

    ByteArrayFilterFactory& operator=(const ByteArrayFilterFactory&) = delete;
    ByteArrayFilterFactory& operator=(ByteArrayFilterFactory&&) = delete;

public:
    [[nodiscard]]
    static std::vector<std::unique_ptr<AbstractByteArrayFilter>> createFilters();

};

#endif
