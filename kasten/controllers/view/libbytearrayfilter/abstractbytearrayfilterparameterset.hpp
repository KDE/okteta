/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTBYTEARRAYFILTERPARAMETERSET_HPP
#define KASTEN_ABSTRACTBYTEARRAYFILTERPARAMETERSET_HPP

#include <QScopedPointer>

class AbstractByteArrayFilterParameterSet
{
protected:
    AbstractByteArrayFilterParameterSet();

public:
    virtual ~AbstractByteArrayFilterParameterSet();

public:
    virtual const char* id() const = 0;

private:
    QScopedPointer<class AbstractByteArrayFilterParameterSetPrivate> const d;
};

#endif
