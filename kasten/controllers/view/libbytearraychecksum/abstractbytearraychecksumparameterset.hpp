/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYCHECKSUMPARAMETERSET_HPP
#define KASTEN_ABSTRACTBYTEARRAYCHECKSUMPARAMETERSET_HPP

#include <QScopedPointer>

class AbstractByteArrayChecksumParameterSet
{
protected:
    AbstractByteArrayChecksumParameterSet();

public:
    virtual ~AbstractByteArrayChecksumParameterSet();

public:
    virtual const char* id() const = 0;

private:
    QScopedPointer<class AbstractByteArrayChecksumParameterSetPrivate> const d;
};

#endif
