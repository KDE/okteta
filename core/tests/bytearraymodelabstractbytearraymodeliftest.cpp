/*
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

std::unique_ptr<AbstractByteArrayModel> ByteArrayModelAbstractByteArrayModelIfTest::createByteArrayModel()
{
    auto byteArrayModel = std::make_unique<ByteArrayModel>(ByteArrayModelSize);
    byteArrayModel->setReadOnly(false);

    return byteArrayModel;
}

void ByteArrayModelAbstractByteArrayModelIfTest::deleteByteArrayModel(std::unique_ptr<AbstractByteArrayModel>&& byteArrayModel)
{
    Q_UNUSED(byteArrayModel)
}

}

QTEST_GUILESS_MAIN(Okteta::ByteArrayModelAbstractByteArrayModelIfTest)

#include "moc_bytearraymodelabstractbytearraymodeliftest.cpp"
