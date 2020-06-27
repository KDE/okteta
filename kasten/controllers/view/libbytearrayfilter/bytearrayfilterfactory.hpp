/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYFILTERFACTORY_HPP
#define KASTEN_BYTEARRAYFILTERFACTORY_HPP

class AbstractByteArrayFilter;
template <class T> class QVector;

class ByteArrayFilterFactory
{
protected:
    ByteArrayFilterFactory();

public:
    static QVector<AbstractByteArrayFilter*> createFilters();

};

#endif
