/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "checksumcalculatejob.hpp"

//
#include <abstractbytearraychecksumalgorithm.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QCoreApplication>

namespace Kasten {

void ChecksumCalculateJob::exec()
{
    // check
    if (!mByteArrayModel || !mSelection.isValid()) {
        // reset
        mChecksum->clear();

        deleteLater(); // TODO: could be reused on next operation

        return;
    }

    connect(mAlgorithm, &AbstractByteArrayChecksumAlgorithm::calculatedBytes, this, &ChecksumCalculateJob::onCalculatedBytes);

    mAlgorithm->calculateChecksum(mChecksum, mByteArrayModel, mSelection);

    deleteLater(); // TODO: could be reused on next operation
}

void ChecksumCalculateJob::onCalculatedBytes()
{
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
}

}
