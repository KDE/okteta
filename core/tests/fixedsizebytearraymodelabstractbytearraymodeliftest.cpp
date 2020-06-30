/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "fixedsizebytearraymodelabstractbytearraymodeliftest.hpp"

// test object
#include <fixedsizebytearraymodel.hpp>
// Qt
#include <QTest>

namespace Okteta {

static constexpr Size FixedSizeByteArrayModelSize = 60;

bool FixedSizeByteArrayModelAbstractByteArrayModelIfTest::byteArrayModelSizeCanBeChanged() const
{
    return false;
}

AbstractByteArrayModel* FixedSizeByteArrayModelAbstractByteArrayModelIfTest::createByteArrayModel()
{
    return new FixedSizeByteArrayModel(FixedSizeByteArrayModelSize);
}

void FixedSizeByteArrayModelAbstractByteArrayModelIfTest::deleteByteArrayModel(AbstractByteArrayModel* byteArrayModel)
{
    delete byteArrayModel;
}

}

QTEST_GUILESS_MAIN(Okteta::FixedSizeByteArrayModelAbstractByteArrayModelIfTest)
