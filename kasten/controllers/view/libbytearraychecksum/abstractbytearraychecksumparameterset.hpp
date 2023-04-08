/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYCHECKSUMPARAMETERSET_HPP
#define KASTEN_ABSTRACTBYTEARRAYCHECKSUMPARAMETERSET_HPP

// Std
#include <memory>

class AbstractByteArrayChecksumParameterSet
{
protected:
    AbstractByteArrayChecksumParameterSet();

public:
    AbstractByteArrayChecksumParameterSet(const AbstractByteArrayChecksumParameterSet&) = delete;

    virtual ~AbstractByteArrayChecksumParameterSet();

    AbstractByteArrayChecksumParameterSet& operator=(const AbstractByteArrayChecksumParameterSet&) = delete;

public:
    virtual const char* id() const = 0;

private:
    const std::unique_ptr<class AbstractByteArrayChecksumParameterSetPrivate> d;
};

#endif
