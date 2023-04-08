/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYFILTERPARAMETERSET_HPP
#define KASTEN_ABSTRACTBYTEARRAYFILTERPARAMETERSET_HPP

// Std
#include <memory>

class AbstractByteArrayFilterParameterSet
{
protected:
    AbstractByteArrayFilterParameterSet();

public:
    AbstractByteArrayFilterParameterSet(const AbstractByteArrayFilterParameterSet&) = delete;

    virtual ~AbstractByteArrayFilterParameterSet();

    AbstractByteArrayFilterParameterSet& operator=(const AbstractByteArrayFilterParameterSet&) = delete;

public:
    virtual const char* id() const = 0;

private:
    const std::unique_ptr<class AbstractByteArrayFilterParameterSetPrivate> d;
};

#endif
