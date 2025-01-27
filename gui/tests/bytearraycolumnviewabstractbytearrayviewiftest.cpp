/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycolumnviewabstractbytearrayviewiftest.hpp"

// test object
#include <bytearraycolumnview.hpp>
// Qt
#include <QTest>

namespace Okteta {

AbstractByteArrayView* ByteArrayColumnViewAbstractByteArrayViewIfTest::createByteArrayView()
{
    auto* byteArrayView = new ByteArrayColumnView();

    return byteArrayView;
}

void ByteArrayColumnViewAbstractByteArrayViewIfTest::deleteByteArrayView(AbstractByteArrayView* byteArrayView)
{
    delete byteArrayView;
}

}

QTEST_MAIN(Okteta::ByteArrayColumnViewAbstractByteArrayViewIfTest)

#include "moc_bytearraycolumnviewabstractbytearrayviewiftest.cpp"
