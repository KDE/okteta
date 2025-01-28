/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrowviewabstractbytearrayviewiftest.hpp"

// test object
#include <bytearrayrowview.hpp>
// Qt
#include <QTest>

namespace Okteta {

std::unique_ptr<AbstractByteArrayView> ByteArrayRowViewAbstractByteArrayViewIfTest::createByteArrayView()
{
    auto byteArrayView = std::make_unique<ByteArrayRowView>();

    return byteArrayView;
}

void ByteArrayRowViewAbstractByteArrayViewIfTest::deleteByteArrayView(std::unique_ptr<AbstractByteArrayView>&& byteArrayView)
{
    Q_UNUSED(byteArrayView)
}

}

QTEST_MAIN(Okteta::ByteArrayRowViewAbstractByteArrayViewIfTest)

#include "moc_bytearrayrowviewabstractbytearrayviewiftest.cpp"
