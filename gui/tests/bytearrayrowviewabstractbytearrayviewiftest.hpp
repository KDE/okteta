/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef BYTEARRAYROWVIEWABSTRACTBYTEARRAYVIEWIFTEST_HPP
#define BYTEARRAYROWVIEWABSTRACTBYTEARRAYVIEWIFTEST_HPP

// test
#include "abstractbytearrayviewiftest.hpp"

namespace Okteta {

class ByteArrayRowViewAbstractByteArrayViewIfTest : public AbstractByteArrayViewIfTest
{
    Q_OBJECT

protected: // AbstractByteArrayViewIfTest API
    std::unique_ptr<AbstractByteArrayView> createByteArrayView() override;
    void deleteByteArrayView(std::unique_ptr<AbstractByteArrayView>&& byteArrayView) override;
};

}

#endif
