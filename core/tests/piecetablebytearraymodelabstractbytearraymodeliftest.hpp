/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef PIECETABLEBYTEARRAYMODELABSTRACTBYTEARRAYMODELIFTEST_HPP
#define PIECETABLEBYTEARRAYMODELABSTRACTBYTEARRAYMODELIFTEST_HPP

// test
#include "abstractbytearraymodeliftest.hpp"

namespace Okteta {

class PieceTableByteArrayModelAbstractByteArrayModelIfTest : public AbstractByteArrayModelIfTest
{
    Q_OBJECT

protected: // AbstractByteArrayModelIfTest API
    std::unique_ptr<AbstractByteArrayModel> createByteArrayModel() override;
    void deleteByteArrayModel(std::unique_ptr<AbstractByteArrayModel>&& byteArrayModel) override;
};

}

#endif
