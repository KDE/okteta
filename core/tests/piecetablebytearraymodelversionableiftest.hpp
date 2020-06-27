/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef PIECETABLEBYTEARRAYMODELVERSIONABLEIFTEST_HPP
#define PIECETABLEBYTEARRAYMODELVERSIONABLEIFTEST_HPP

// test
#include "versionableiftest.hpp"

namespace Okteta {

class PieceTableByteArrayModelVersionableIfTest : public VersionableIfTest
{
    Q_OBJECT

protected: // VersionableIfTest API
    QObject* createVersionable() override;
    void deleteVersionable(QObject* versionable) override;
};

}

#endif
