/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycolumnviewabstractbytearrayviewiftest.hpp"

// test object
#include <bytearraycolumnview.hpp>
// Qt
#include <QTest>

namespace Okteta {

std::unique_ptr<AbstractByteArrayView> ByteArrayColumnViewAbstractByteArrayViewIfTest::createByteArrayView()
{
    auto byteArrayView = std::make_unique<ByteArrayColumnView>();

    return byteArrayView;
}

void ByteArrayColumnViewAbstractByteArrayViewIfTest::deleteByteArrayView(std::unique_ptr<AbstractByteArrayView>&& byteArrayView)
{
    Q_UNUSED(byteArrayView)
}

}

QTEST_MAIN(Okteta::ByteArrayColumnViewAbstractByteArrayViewIfTest)

#include "moc_bytearraycolumnviewabstractbytearrayviewiftest.cpp"
