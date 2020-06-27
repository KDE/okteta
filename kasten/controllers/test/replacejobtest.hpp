/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef REPLACEJOBTEST_HPP
#define REPLACEJOBTEST_HPP

// Qt
#include <QObject>

namespace Okteta { class PieceTableByteArrayModel; }
class QByteArray;

class ReplaceJobTest : public QObject
{
    Q_OBJECT

private Q_SLOTS: // test functions
    void testReplace_data();
    void testReplace();

private:
    void compare(const Okteta::PieceTableByteArrayModel* byteArrayModel, const QByteArray& data);
};

#endif
