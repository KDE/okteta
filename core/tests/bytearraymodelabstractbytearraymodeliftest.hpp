/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef BYTEARRAYMODELABSTRACTBYTEARRAYMODELIFTEST_HPP
#define BYTEARRAYMODELABSTRACTBYTEARRAYMODELIFTEST_HPP

// test
#include "abstractbytearraymodeliftest.hpp"

namespace Okteta {

class ByteArrayModelAbstractByteArrayModelIfTest : public AbstractByteArrayModelIfTest
{
    Q_OBJECT

protected: // AbstractByteArrayModelIfTest API
    AbstractByteArrayModel* createByteArrayModel() override;
    void deleteByteArrayModel(AbstractByteArrayModel* byteArrayModel) override;
};

}

#endif
