/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetablebytearraymodelversionableiftest.hpp"

// test object
#include <piecetablebytearraymodel.hpp>
// Qt
#include <QTest>

namespace Okteta {

static constexpr Size ByteArrayModelSize = 60;

QObject* PieceTableByteArrayModelVersionableIfTest::createVersionable()
{
    auto* byteArrayModel = new PieceTableByteArrayModel(ByteArrayModelSize);
    byteArrayModel->setReadOnly(false);

    return byteArrayModel;
}

void PieceTableByteArrayModelVersionableIfTest::deleteVersionable(QObject* versionable)
{
    delete versionable;
}

}

QTEST_GUILESS_MAIN(Okteta::PieceTableByteArrayModelVersionableIfTest)
