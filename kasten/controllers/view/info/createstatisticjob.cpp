/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "createstatisticjob.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QCoreApplication>

namespace Kasten {

static constexpr int StatisticBlockSize = 100000;

int CreateStatisticJob::exec()
{
    // reset
    memset(mByteCount, 0, 256 * sizeof(int));

    const Okteta::Address last = mByteArrayModel ? mSelection.end() : -1;
    Okteta::Address i = mByteArrayModel ? mSelection.start() : 0;
    Okteta::Address blockEnd = i;
    while (i <= last) {
        blockEnd += StatisticBlockSize;
        if (blockEnd > last) {
            blockEnd = last;
        }

        for (; i <= blockEnd; ++i) {
            ++mByteCount[mByteArrayModel->byte(i)];
        }

        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    deleteLater(); // TODO: could be reused on next search

    return (mByteArrayModel ? mSelection.width() : -1);
}

}
