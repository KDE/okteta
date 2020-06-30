/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCHECKSUMPARAMETERSETEDITFACTORY_HPP
#define KASTEN_BYTEARRAYCHECKSUMPARAMETERSETEDITFACTORY_HPP

class AbstractByteArrayChecksumParameterSetEdit;

class ByteArrayChecksumParameterSetEditFactory
{
protected:
    ByteArrayChecksumParameterSetEditFactory();

public:
    static AbstractByteArrayChecksumParameterSetEdit* createEdit(const char* id);
};

#endif
