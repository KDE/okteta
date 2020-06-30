/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraymodelabstractbytearraymodeliftest.hpp"

// test object
#include <bytearraymodel.hpp>
// Qt
#include <QTest>

namespace Okteta {

static constexpr Size ByteArrayModelSize = 60;

AbstractByteArrayModel* ByteArrayModelAbstractByteArrayModelIfTest::createByteArrayModel()
{
    auto* byteArrayModel = new ByteArrayModel(ByteArrayModelSize);
    byteArrayModel->setReadOnly(false);

    return byteArrayModel;
}

void ByteArrayModelAbstractByteArrayModelIfTest::deleteByteArrayModel(AbstractByteArrayModel* byteArrayModel)
{
    delete byteArrayModel;
}

}

QTEST_GUILESS_MAIN(Okteta::ByteArrayModelAbstractByteArrayModelIfTest)
