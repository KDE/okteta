/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCHECKSUMALGORITHMFACTORY_HPP
#define KASTEN_BYTEARRAYCHECKSUMALGORITHMFACTORY_HPP

class AbstractByteArrayChecksumAlgorithm;
template <class T> class QVector;

class ByteArrayChecksumAlgorithmFactory
{
protected:
    ByteArrayChecksumAlgorithmFactory();

public:
    static QVector<AbstractByteArrayChecksumAlgorithm*> createAlgorithms();

};

#endif
