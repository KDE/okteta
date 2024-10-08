/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetablebytearraymodelabstractbytearraymodeliftest.hpp"

// test object
#include <piecetablebytearraymodel.hpp>
// Qt
#include <QTest>

namespace Okteta {

static constexpr Size ByteArrayModelSize = 60;

std::unique_ptr<AbstractByteArrayModel> PieceTableByteArrayModelAbstractByteArrayModelIfTest::createByteArrayModel()
{
    auto byteArrayModel = std::make_unique<PieceTableByteArrayModel>(ByteArrayModelSize);
    byteArrayModel->setReadOnly(false);

    return byteArrayModel;
}

void PieceTableByteArrayModelAbstractByteArrayModelIfTest::deleteByteArrayModel(std::unique_ptr<AbstractByteArrayModel>&& byteArrayModel)
{
    Q_UNUSED(byteArrayModel)
}

}

QTEST_GUILESS_MAIN(Okteta::PieceTableByteArrayModelAbstractByteArrayModelIfTest)

#include "moc_piecetablebytearraymodelabstractbytearraymodeliftest.cpp"
