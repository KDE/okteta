/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYFILTERPARAMETERSETEDITFACTORY_HPP
#define KASTEN_BYTEARRAYFILTERPARAMETERSETEDITFACTORY_HPP

class AbstractByteArrayFilterParameterSetEdit;
class QByteArray;

class ByteArrayFilterParameterSetEditFactory
{
protected:
    ByteArrayFilterParameterSetEditFactory();

public:
    static AbstractByteArrayFilterParameterSetEdit* createEdit(const char* id);
};

#endif
